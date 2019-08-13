#include "widgets/network.h"

#include <QBoxLayout>
#include <QLabel>

#include <optional>
#include <iostream>
#include <string>

using namespace std;

using namespace TDB::Widgets;

Network::Network(QWidget *parent, const std::shared_ptr<Session> session, string name) : QWidget(parent), _session(session), _name(name) {
  rx = new NetworkGraph(this, session, [this](const network_util_t &util) -> uint64_t {
    if(this->_name != "") return util.ifs.at(this->_name).rx;
    uint64_t total = 0;
    for(const auto &[name, if_stat] : util.ifs)
      total += if_stat.rx;
    return total;
  }, "859900");

  tx = new NetworkGraph(this, session, [this](const network_util_t &util) -> uint64_t {
    if(this->_name != "") return util.ifs.at(this->_name).tx;
    uint64_t total = 0;
    for(const auto &[name, if_stat] : util.ifs)
      total += if_stat.tx;
    return total;
  }, "cb4b16");

  auto layout = new QBoxLayout(QBoxLayout::LeftToRight);
  layout->addWidget(rx, 1);
  layout->addWidget(tx, 1);
  this->setLayout(layout);
}

void Network::update() {
  QWidget::update();
  rx->update();
}
