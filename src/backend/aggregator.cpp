#include "backend/aggregator.h"

#include <iostream>

using namespace TDB;
using namespace std;

core_util_t core_stat_t::operator-(const core_stat_t &ano) const {
  return core_util_t {
    .user    = user    - ano.user,
      .nice    = nice    - ano.nice,
      .system  = nice    - ano.nice,
      .idle    = idle    - ano.idle,
      .iowait  = iowait  - ano.iowait,
      .irq     = irq     - ano.irq,
      .softirq = softirq - ano.softirq,
  };
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
    .cpu_util = _last_cpu_stat - cpu_stat,
  };

  _last_cpu_stat = cpu_stat;

  return result;
}

cpu_stat_t Aggregator::fetch_cpu_stat() {
  string stat = _exec->exec("cat /proc/stat");
  
  // TODO: parse
}
