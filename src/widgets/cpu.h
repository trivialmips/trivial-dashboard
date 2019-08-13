#ifndef __CPU_H__
#define __CPU_H__

#include "state.h"

#include <memory>

#include <QWidget>
#include <QLayout>

namespace TDB::Widgets {
  class CPU : public QWidget {
    Q_OBJECT

    public:
      CPU(QWidget *parent, const std::shared_ptr<Session> session, int core = -1);

    protected:
      void paintEvent(QPaintEvent *e);

    private:
      const std::shared_ptr<Session> _session;
      int _core;
  };
}
#endif // __CPU_H__
