#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <string>

namespace TDB {
  class Executor {
    public:
      virtual ~Executor() {}
      virtual std::string exec(std::string cmd) = 0;
  };

  class LocalExecutor : public Executor {
    public:
      std::string exec(std::string cmd) override;
  };

  // TODO: impl SSHExecutor
}

#endif // __EXECUTOR_H__
