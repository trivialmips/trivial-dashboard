#ifndef __FRAME_H__
#define __FRAME_H__

#include "state.h"

#include "widgets/cpu.h"

#include <QMainWindow>

namespace TDB {
  class Frame : public QMainWindow {
    Q_OBJECT

    public:
      Frame();
      State state;

    public slots:
      void exit();
      void tick();

    private:
      Widgets::CPU *cpu;
  };
}

#endif // __FRAME_H__
