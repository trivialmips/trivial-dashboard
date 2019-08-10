#include "frame.h"

#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  TDB::Frame frame;
  frame.show();

  return app.exec();
}
