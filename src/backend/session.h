#ifndef __SESSION_H__
#define __SESSION_H__

#include "backend/executor.h"
#include "backend/aggregator.h"

#include <memory>

namespace TDB {
  class Session {
    public:
      Session(std::unique_ptr<Executor> exec);
    private:
      std::unique_ptr<Aggregator> _aggr;
  };
}

#endif // __SESSION_H__
