#include "frame.h"

#include <QBoxLayout>
#include <QWidget>
#include <QLabel>

using namespace TDB;

Frame::Frame() {
  this->setWindowTitle("Trivial-Dashboard");

  auto hi = new QLabel(this);
  auto hiPalette = hi->palette();
  hiPalette.setColor(hi->backgroundRole(), Qt::red);
  hi->setPalette(hiPalette);
  hi->setAutoFillBackground(true);
  hi->setText("Hello, Nontrivial-MIPS!");

  auto layout = new QBoxLayout(QBoxLayout::LeftToRight);
  layout->setSpacing(20);

  layout->addWidget(hi);

  auto container = new QWidget(this);
  container->setLayout(layout);
  this->setCentralWidget(container);
}
