#ifndef __AGGREGATOR_H__
#define __AGGREGATOR_H__

#include "backend/executor.h"
#include <memory>
#include <vector>

namespace TDB {
  struct core_util_t {
    uint64_t total, user, nice, system, idle, iowait, irq, softirq;
  };

  struct cpu_util_t {
    std::vector<core_util_t> cores;
    uint64_t btime;
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

  // Aggregator tick
  struct tick_t {
    cpu_util_t cpu_util;
  };

  class Aggregator {
    public:
      Aggregator(std::unique_ptr<Executor> exec);
      tick_t tick();
    private:
      cpu_stat_t fetch_cpu_stat();

      std::unique_ptr<Executor> _exec;
      cpu_stat_t _last_cpu_stat;
  };
}

#endif // __AGGREGATOR_H__
