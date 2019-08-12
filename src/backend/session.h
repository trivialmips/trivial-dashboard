#ifndef __SESSION_H__
#define __SESSION_H__

#include "backend/executor.h"
#include "backend/aggregator.h"

#include <memory>
#include <optional>
#include <deque>

namespace TDB {
  class Session {
    public:
      Session(std::unique_ptr<Executor> exec);
      void update();
      const std::deque<std::optional<tick_t>>& get_ticks() const;
    private:
      std::unique_ptr<Aggregator> _aggr;
      std::deque<std::optional<tick_t>> ticks;
  };
}

#endif // __SESSION_H__
