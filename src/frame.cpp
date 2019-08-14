#include "frame.h"
#include "widgets/legend.h"
#include "widgets/sshterm.h"
#include "util.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QColor>
#include <QPushButton>
#include <QKeyEvent>

#include <libssh/libssh.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace TDB;
using namespace std;

Widgets::Legend * create_cpu_legend(QWidget *parent) {
  return new Widgets::Legend(parent, {
    { "user", "268bd2" },
    { "nice", "2aa198" },
    { "system", "b58900" },
    { "idle", "859900" },
    { "iowait", "dc322f" },
    { "irq", "d33682" },
    { "softirq", "6c71c4" },
  });
}

Widgets::Legend * create_network_legend(QWidget *parent) {
  return new Widgets::Legend(parent, {
    { "rx", "859900" },
    { "tx", "cb4b16" },
  });
}

Frame::Frame(std::optional<tuple<std::string, std::string, int>> remote, int session_size) {
  this->installEventFilter(this);
  this->setWindowTitle("Trivial-Dashboard");

  this->title = new QLabel(this);
  this->title->setText("Trivial-Dashboard");

  /*
  auto hi = new QLabel(this);
  hi->setAlignment(Qt::AlignCenter);
  */

  this->state.set_default_size(session_size);
  if(remote) {
    auto [chan, ssh_mutex] = this->state.open_ssh_session(get<0>(*remote), get<1>(*remote), get<2>(*remote));
    term_widget = new Widgets::SSHTerm(this, chan, ssh_mutex);
  } else {
    this->state.open_local_session();
    term_widget = new QTermWidget(this);
  }

  // Init term
  term_widget->setScrollBarPosition(QTermWidget::ScrollBarRight);
  term_widget->setColorScheme("Solarized");
  QFont term_font = this->font();
  term_font.setPixelSize(14);
  term_widget->setFont(term_font);

  connect(term_widget, SIGNAL(finished()), this, SLOT(exit()));
  connect(term_widget, SIGNAL(termKeyPressed(QKeyEvent *)), this, SLOT(termKeyPressed(QKeyEvent *)));

  auto timer = new QTimer(this);
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
  timer->start();

  auto btn = new QPushButton("Get my out of here");
  connect(btn, SIGNAL(clicked()), this, SLOT(exit()));

  home_widget = new QWidget(this);
  cpu_widget = new QWidget(this);
  network_widget = new QWidget(this);
  
  home_layout = new QBoxLayout(QBoxLayout::TopToBottom);
  home_layout->setSpacing(5);

  // Top
  auto top = new QGridLayout(this);
  auto cpu_region = new QBoxLayout(QBoxLayout::TopToBottom);
  cpu_region->setSpacing(5);
  cpu = new Widgets::CPU(this, this->state.active_session());
  auto cpu_legend = create_cpu_legend(this);

  cpu_region->addWidget(cpu, 1);
  cpu_region->addWidget(cpu_legend);
  top->addLayout(cpu_region, 0, 0);

  auto network_region = new QBoxLayout(QBoxLayout::TopToBottom);
  network_region->setSpacing(5);
  network = new Widgets::Network(this, this->state.active_session());
  auto network_legend = create_network_legend(this);
  network_region->addWidget(network, 1);
  network_region->addWidget(network_legend);
  top->addLayout(network_region, 1, 0);

  top->setRowStretch(0, 1);
  top->setRowStretch(1, 1);
  home_layout->addLayout(top);

  home_widget->setLayout(home_layout);

  container = new QStackedWidget(this);
  container->addWidget(home_widget);
  container->addWidget(cpu_widget);
  container->addWidget(network_widget);
  container->addWidget(term_widget);

  home_widget->show();
  cpu_widget->hide();
  network_widget->hide();
  term_widget->hide();

  auto palette = container->palette();
  palette.setColor(container->backgroundRole(), "#002b36");
  palette.setColor(container->foregroundRole(), "#839496");
  container->setPalette(palette);
  container->setAutoFillBackground(true);

  this->setCentralWidget(container);
  auto size = QApplication::desktop()->availableGeometry(this).size();

  this->resize(640, 480);
  home_widget->setFixedSize(640, 480);
  cpu_widget->setFixedSize(640, 480);
  network_widget->setFixedSize(640, 480);
  term_widget->setFixedSize(640, 480);

  stringstream ss;
  ss<<size.width()<<"x"<<size.height();

  // hi->setText(QString("Hello, Nontrivial-MIPS!\n") + ss.str().c_str());

}

bool Frame::eventFilter(QObject *obj, QEvent *ev) {
  if(ev->type() == QEvent::KeyPress) {
    QKeyEvent *keyEv = static_cast<QKeyEvent *>(ev);
    if(keyEv->key() == Qt::Key_C)
      this->gotoCPUView();
    else if(keyEv->key() == Qt::Key_N)
      this->gotoNetworkView();
    else if(keyEv->key() == Qt::Key_H)
      this->gotoHomeView();
    else if(keyEv->key() == Qt::Key_T)
      this->gotoTermView();
    else if(keyEv->key() == Qt::Key_Q)
      this->exit();

    return true;
  }

  return QObject::eventFilter(obj, ev);
}

void Frame::exit() {
  QApplication::quit();
}

void Frame::tick() {
  this->state.update();
  cpu->update();
  network->update();

  for(const auto &core : cores) core->update();
  for(const auto &ifw: ifs) ifw->update();

  this->updateCoreLabels();
  this->updateIfLabels();
}

void Frame::gotoCPUView() {
  this->title->setText(QString((string("Trivial-Dashboard") + "| CPU Overview").c_str()));

  if(this->cpu_layout == nullptr) {
    // Load up cpu layout
    cpu_layout = new QGridLayout();
    cpu_layout->setSpacing(5);
    const int core_count = this->state.active_session()->count_cores();
    const int rows = ceil(sqrt(core_count));
    const int cols = ceil(((double) core_count) / rows);

    for(int i = 0; i<core_count; ++i) {
      const int row = i / cols;
      const int col = i % cols;

      Widgets::CPU *core = new Widgets::CPU(this, this->state.active_session(), i);
      cpu_layout->addWidget(core, row*2, col);
      cores.push_back(core);

      auto label = new QLabel(this);
      label->setAlignment(Qt::AlignCenter);
      QFont font = label->font();
      font.setPixelSize(10);
      label->setFont(font);
      cpu_layout->addWidget(label, row*2+1, col);
      core_labels.push_back(label);
    }

    for(int i = 0; i < rows; ++i) {
      cpu_layout->setRowStretch(i*2, 1);
      cpu_layout->setRowStretch(i*2+1, 0);
    }
    for(int i = 0; i < cols; ++i) cpu_layout->setColumnStretch(i, 1);

    // Legend
    auto cpu_legend = create_cpu_legend(this);

    cpu_layout->addWidget(cpu_legend, rows*2, 0, 1, cols);
    cpu_widget->setLayout(cpu_layout);

    this->updateCoreLabels();
  }

  this->setFocus(Qt::MouseFocusReason);

  home_widget->hide();
  cpu_widget->show();
  network_widget->hide();
  term_widget->hide();
}

void Frame::gotoNetworkView() {
  this->title->setText(QString((string("Trivial-Dashboard") + "| Network Overview").c_str()));

  if(this->network_layout == nullptr) {
    network_layout = new QGridLayout();
    network_layout->setSpacing(5);
    const int if_count = this->state.active_session()->count_ifs();
    const int rows = ceil(sqrt(if_count));
    const int cols = ceil(((double) if_count) / rows);

    int counter = 0;
    const auto &last_tick = this->state.active_session()->get_ticks().back()->network_util;

    for(const auto &[name, stat] : last_tick.ifs) {
      const int row = counter / cols;
      const int col = counter % cols;
      ++counter;

      if_names.push_back(name);

      Widgets::Network *ifw = new Widgets::Network(this, this->state.active_session(), name);
      network_layout->addWidget(ifw, row*2, col);
      ifs.push_back(ifw);

      auto label = new QLabel(this);
      label->setAlignment(Qt::AlignCenter);
      QFont font = label->font();
      font.setPixelSize(10);
      label->setFont(font);
      network_layout->addWidget(label, row*2+1, col);
      if_labels.push_back(label);
    }

    for(int i = 0; i < rows; ++i) {
      network_layout->setRowStretch(i*2, 1);
      network_layout->setRowStretch(i*2+1, 0);
    }
    for(int i = 0; i < cols; ++i) network_layout->setColumnStretch(i, 1);

    // Legend
    auto network_legend = create_network_legend(this);

    network_layout->addWidget(network_legend, rows*2, 0, 1, cols);
    network_widget->setLayout(network_layout);

    this->updateIfLabels();
  }

  this->setFocus(Qt::MouseFocusReason);

  home_widget->hide();
  cpu_widget->hide();
  network_widget->show();
  term_widget->hide();
}

void Frame::gotoHomeView() {
  this->title->setText(QString(string("Trivial-Dashboard").c_str()));

  this->setFocus(Qt::MouseFocusReason);

  home_widget->show();
  cpu_widget->hide();
  network_widget->hide();
  term_widget->hide();
}

void Frame::gotoTermView() {
  this->title->setText(QString((string("Trivial-Dashboard") + " | Term").c_str()));

  home_widget->hide();
  cpu_widget->hide();
  network_widget->hide();
  term_widget->show();

  term_widget->setFocus(Qt::MouseFocusReason);
}

void Frame::updateCoreLabels() {
  if(cpu_layout == nullptr) return;

  auto last_tick = this->state.active_session()->get_ticks().back();

  for(int i = 0; i<core_labels.size(); ++i) {
    stringstream label;
    label<<"CPU "<<i;

    if(last_tick) {
      long double util = 1 - ((long double) last_tick->cpu_util.cores[i].idle) / last_tick->cpu_util.cores[i].total;
      util *= 100;
      label<<" ("<<fixed<<setprecision(2)<<util<<"%)";
    }

    core_labels[i]->setText(label.str().c_str());
  }
}

void Frame::updateIfLabels() {
  if(network_layout == nullptr) return;

  auto last_tick = this->state.active_session()->get_ticks().back();

  for(int i = 0; i<if_labels.size(); ++i) {
    stringstream label;
    label<<if_names[i];

    if(last_tick) {
      const auto &stat = last_tick->network_util.ifs[if_names[i]];
      label<<" ("<<transfer_text(stat.rx)<<"↓ "<<transfer_text(stat.tx)<<"↑)";
    }

    if_labels[i]->setText(label.str().c_str());
  }
}

void Frame::termKeyPressed(QKeyEvent *ev) {
  qDebug()<<ev;
  if(ev->key() == Qt::Key_H && (ev->modifiers() & Qt::ControlModifier) && (ev->modifiers() & Qt::ShiftModifier))
    this->gotoHomeView();
}
