#include "frame.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QColor>
#include <QPushButton>

#include <iostream>
#include <sstream>

using namespace TDB;
using namespace std;

Frame::Frame() {
  this->setWindowTitle("Trivial-Dashboard");

  /*
  auto hi = new QLabel(this);
  hi->setAlignment(Qt::AlignCenter);
  */

  this->state.open_local_session();

  auto timer = new QTimer(this);
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
  timer->start();

  cpu = new Widgets::CPU(this->state.get_sessions()[0]);

  auto btn = new QPushButton("Get my out of here");
  connect(btn, SIGNAL(clicked()), this, SLOT(exit()));

  auto layout = new QBoxLayout(QBoxLayout::TopToBottom);
  layout->setSpacing(20);

  layout->addWidget(cpu);
  layout->addWidget(btn);

  auto container = new QWidget(this);
  container->setLayout(layout);
  auto palette = container->palette();
  palette.setColor(container->backgroundRole(), "#002b36");
  palette.setColor(container->foregroundRole(), "#839496");
  container->setPalette(palette);
  container->setAutoFillBackground(true);

  this->setCentralWidget(container);
  auto size = QApplication::desktop()->availableGeometry(this).size();
  // this->resize(size);
  this->resize(640, 480);

  stringstream ss;
  ss<<size.width()<<"x"<<size.height();

  // hi->setText(QString("Hello, Nontrivial-MIPS!\n") + ss.str().c_str());
}

void Frame::exit() {
  QApplication::quit();
}

void Frame::tick() {
  this->state.update();
  cpu->update();
}
