#ifndef __FRAME_H__
#define __FRAME_H__

#include <QMainWindow>

namespace TDB {
  class Frame : public QMainWindow {
    Q_OBJECT

    public:
      Frame();

    public slots:
      void exit();
  };
}

#endif // __FRAME_H__
