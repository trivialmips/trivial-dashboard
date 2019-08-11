#include "frame.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QColor>
#include <QPushButton>

#include <iostream>
#include <sstream>

using namespace TDB;
using namespace std;

Frame::Frame() {
  this->setWindowTitle("Trivial-Dashboard");

  auto hi = new QLabel(this);
  hi->setAlignment(Qt::AlignCenter);

  auto btn = new QPushButton("Get my out of here");
  connect(btn, SIGNAL(clicked()), this, SLOT(exit()));

  auto layout = new QBoxLayout(QBoxLayout::TopToBottom);
  layout->setSpacing(20);

  layout->addWidget(hi);
  layout->addWidget(btn);

  auto container = new QWidget(this);
  container->setLayout(layout);
  auto palette = hi->palette();
  palette.setColor(hi->backgroundRole(), "#002b36");
  palette.setColor(hi->foregroundRole(), "#839496");
  container->setPalette(palette);
  container->setAutoFillBackground(true);

  this->setCentralWidget(container);
  auto size = QApplication::desktop()->availableGeometry(this).size();
  this->resize(size);

  stringstream ss;
  ss<<size.width()<<"x"<<size.height();

  hi->setText(QString("Hello, Nontrivial-MIPS!\n") + ss.str().c_str());
}

void Frame::exit() {
  QApplication::quit();
}
