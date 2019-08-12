#ifndef __STATE_H__
#define __STATE_H__

#include "backend/session.h"

#include <memory>
#include <vector>
#include <QObject>

namespace TDB {
  class Frame; // Resolve circular dependency

  class State : public QObject{
    Q_OBJECT // We need QTimer, unfortunately

    friend Frame;

    public:
      void start_timer();
      void open_local_session();

    private:
      std::vector<std::unique_ptr<Session>> sessions;
      Frame *frame;
  };
}

#endif // __STATE_H__
