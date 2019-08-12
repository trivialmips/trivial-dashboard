#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <string>
#include <libssh/libssh.h>

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

  class SSHExecutor : public Executor {
    public:
      SSHExecutor(std::string host, std::string user, int port = 22);
      ~SSHExecutor();
      std::string exec(std::string cmd) override;
    private:
      ssh_session _ssh;
  };
}

#endif // __EXECUTOR_H__
