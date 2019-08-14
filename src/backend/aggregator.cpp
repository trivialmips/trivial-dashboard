#include "backend/aggregator.h"

#include <iostream>
#include <sstream>

using namespace TDB;
using namespace std;

core_util_t core_stat_t::operator-(const core_stat_t &ano) const {
  core_util_t result{
    .user    = user    - ano.user,
    .nice    = nice    - ano.nice,
    .system  = nice    - ano.nice,
    .idle    = idle    - ano.idle,
    .iowait  = iowait  - ano.iowait,
    .irq     = irq     - ano.irq,
    .softirq = softirq - ano.softirq,
  };

  result.total = result.user + result.nice + result.system
    + result.idle + result.iowait + result.irq + result.softirq;

  return result;
}

cpu_util_t cpu_stat_t::operator-(const cpu_stat_t &ano) const {
  // Assumes the core count doesn't change, or we are extremely screwed

  cpu_util_t result;
  result.cores.reserve(cores.size());

  for(int i = 0; i < cores.size(); ++i)
    result.cores.push_back(cores[i] - ano.cores[i]);

  result.btime = btime;

  return result;
}

if_util_t if_stat_t::operator-(const if_stat_t &ano) const {
  if_util_t result{
    .rx = rx - ano.rx,
    .tx = tx - ano.tx,
  };

  return result;
}

network_util_t network_stat_t::operator-(const network_stat_t &ano) const {
  // Assumes the core count doesn't change, or we are extremely screwed

  network_util_t result;

  for(const auto &[name, if_stat] : ifs)
    result.ifs.insert({ name, if_stat - ano.ifs.at(name) });

  return result;
}

Aggregator::Aggregator(std::unique_ptr<Executor> exec)
  : _exec(std::move(exec)), _last_cpu_stat(fetch_cpu_stat()), _last_network_stat(fetch_network_stat()) {
  }

tick_t Aggregator::tick() {
  cpu_stat_t cpu_stat = this->fetch_cpu_stat();
  network_stat_t network_stat = this->fetch_network_stat();

  tick_t result {
    .cpu_util = cpu_stat - _last_cpu_stat,
    .network_util = network_stat - _last_network_stat,
  };

  _last_cpu_stat = cpu_stat;
  _last_network_stat = std::move(network_stat);

  return result;
}

cpu_stat_t Aggregator::fetch_cpu_stat() {
  string stat = _exec->exec("cat /proc/stat");

  stringstream ss(stat);

  string type;
  string junk;

  cpu_stat_t result;

  while(ss) {
    ss>>type;
    if(!ss) break;

    if(type == "cpu") {
      // Accumulated CPU util, we don't need these
      std::getline(ss, junk);
    } else if(type.find("cpu") == 0) {
      size_t cpuno = std::stoi(type.substr(3));

      if(cpuno >= result.cores.size())
        result.cores.resize(cpuno+1);

      core_stat_t &core = result.cores[cpuno];
      ss>>core.user
        >>core.nice
        >>core.system
        >>core.idle
        >>core.iowait
        >>core.irq
        >>core.softirq;
    } else if(type == "btime") {
      ss>>result.btime;
    } else {
      std::getline(ss, junk);
    }
  }

  return result;
}

network_stat_t Aggregator::fetch_network_stat() {
  string stat = _exec->exec("cat /proc/net/dev | tail +3");
  network_stat_t result;
  stringstream ss(stat);
  string junk, name;

  while(ss) {
    if_stat_t if_stat;
    ss>>name;
    if(!ss) break;

    name.pop_back();

    ss>>if_stat.rx>>junk>>junk>>junk>>junk>>junk>>junk>>junk>>if_stat.tx;
    std::getline(ss, junk);

    result.ifs.insert({ name, if_stat });
  }

  return result;
}
