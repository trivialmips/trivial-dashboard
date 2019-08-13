#include "widgets/legend.h"

#include <QBoxLayout>
#include <QLabel>

using namespace TDB::Widgets;

Legend::Legend(QWidget *parent, std::vector<std::pair<std::string, std::string>> legends) {
  auto layout = new QBoxLayout(QBoxLayout::LeftToRight);

  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(5);
  layout->setAlignment(Qt::AlignCenter);

  // Centering
  layout->addStretch(1);

  for(const auto &[name, color] : legends) {
    auto box = new QWidget(this);
    box->setFixedSize(10, 10);
    auto palette = box->palette();
    palette.setColor(box->backgroundRole(), ("#" + color).c_str());
    box->setPalette(palette);
    box->setAutoFillBackground(true);
    layout->addWidget(box);

    auto label = new QLabel(this);
    label->setText(name.c_str());
    auto font = label->font();
    font.setPixelSize(12);
    label->setFont(font);
    layout->addWidget(label);
    layout->addSpacing(10);
  }

  layout->addSpacing(10);

  // Centering
  layout->addStretch(1);

  this->setLayout(layout);
}
