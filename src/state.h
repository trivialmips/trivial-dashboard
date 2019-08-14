#ifndef __STATE_H__
#define __STATE_H__

#include "backend/session.h"

#include <memory>
#include <vector>
#include <string>
#include <QObject>

namespace TDB {
  class State : public QObject{
    Q_OBJECT // We need QTimer, unfortunately

    public:
      void update();
      void open_local_session();
      void open_ssh_session(std::string host, std::string user, int port = 22);
      void set_default_size(int size);
      std::vector<std::shared_ptr<Session>>& get_sessions();
      void activate(int i);
      std::shared_ptr<Session> active_session();

    private:
      std::vector<std::shared_ptr<Session>> sessions;
      int _default_size = 20;
      int active = -1;
  };
}

#endif // __STATE_H__
