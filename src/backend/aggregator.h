#ifndef __AGGREGATOR_H__
#define __AGGREGATOR_H__

#include "backend/executor.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace TDB {
  struct core_util_t {
    uint64_t total, user, nice, system, idle, iowait, irq, softirq;
  };

  struct cpu_util_t {
    std::vector<core_util_t> cores;
    uint64_t btime;
  };

  struct if_util_t {
    uint64_t rx, tx;
  };

  struct network_util_t {
    std::unordered_map<std::string, if_util_t> ifs;
  };

  // Internal types
  struct core_stat_t {
    uint64_t user, nice, system, idle, iowait, irq, softirq;

    inline
    core_util_t operator-(const core_stat_t &ano) const;
  };

  struct cpu_stat_t {
    std::vector<core_stat_t> cores;
    uint64_t btime;

    inline
    cpu_util_t operator-(const cpu_stat_t &ano) const;
  };

  struct if_stat_t {
    uint64_t rx, tx;

    inline
    if_util_t operator-(const if_stat_t &ano) const;
  };

  struct network_stat_t {
    std::unordered_map<std::string, if_stat_t> ifs;

    inline
    network_util_t operator-(const network_stat_t &ano) const;
  };

  // Aggregator tick
  struct tick_t {
    cpu_util_t cpu_util;
    network_util_t network_util;
  };

  class Aggregator {
    public:
      Aggregator(std::unique_ptr<Executor> exec);
      tick_t tick();
    private:
      cpu_stat_t fetch_cpu_stat();
      network_stat_t fetch_network_stat();

      std::unique_ptr<Executor> _exec;
      cpu_stat_t _last_cpu_stat;
      network_stat_t _last_network_stat;
  };
}

#endif // __AGGREGATOR_H__
