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
  cout<<"SELF: "<<user<<endl;
  cout<<"ANO: "<<ano.user<<endl;
  cout<<"DIFF: "<<result.user<<endl;

  result.total = result.user + result.nice + result.system
    + result.idle + result.iowait + result.irq + result.softirq;

  cout<<"TOTAL: "<<result.total<<endl;

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

Aggregator::Aggregator(std::unique_ptr<Executor> exec)
  : _exec(std::move(exec)), _last_cpu_stat(fetch_cpu_stat()) {
  }

tick_t Aggregator::tick() {
  cpu_stat_t cpu_stat = this->fetch_cpu_stat();

  tick_t result {
    .cpu_util = cpu_stat - _last_cpu_stat,
  };

  _last_cpu_stat = cpu_stat;

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
