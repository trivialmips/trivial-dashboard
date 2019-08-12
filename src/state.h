#ifndef __STATE_H__
#define __STATE_H__

#include "backend/session.h"

#include <memory>
#include <vector>
#include <QObject>

namespace TDB {
  class State : public QObject{
    Q_OBJECT // We need QTimer, unfortunately

    public:
      void update();
      void open_local_session();
      std::vector<std::shared_ptr<Session>>& get_sessions();

    private:
      std::vector<std::shared_ptr<Session>> sessions;
  };
}

#endif // __STATE_H__
