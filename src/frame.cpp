#include "frame.h"

#include <QBoxLayout>
#include <QWidget>
#include <QLabel>

using namespace TDB;

Frame::Frame() {
  this->setWindowTitle("Trivial-Dashboard");

  auto hi = new QLabel(this);
  hi->setText("Hello, Nontrivial-MIPS!");

  auto layout = new QBoxLayout(QBoxLayout::LeftToRight);
  layout->setSpacing(20);

  layout->addWidget(hi);

  auto container = new QWidget(this);
  container->setLayout(layout);
  this->setCentralWidget(container);
}
