#include "widgets/cpu.h"

#include <QPainter>
#include <QLabel>

#include <optional>
#include <iostream>

#define LINE_STROKE 2

using namespace std;

using namespace TDB::Widgets;

CPU::CPU(QWidget *parent, const std::shared_ptr<Session> session, int core) : QWidget(parent), _session(session), _core(core) {}

long double _total, _req; // temp var
#define FETCH_FIELD(util, no, field, frac) \
  _total = 0; \
  if(no == -1) for(const auto &c : util.cores) _total += c.total; \
  else _total = util.cores[no].total; \
  _req = 0; \
  if(no == -1) for(const auto &c : util.cores) _req += c.field; \
  else _req = util.cores[no].field; \
  frac = _req / _total;

void CPU::paintEvent(QPaintEvent *e) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  QRect cr = contentsRect();

  QBrush bgbrush(QColor("#073642"));

  painter.setPen(Qt::NoPen);
  painter.setBrush(bgbrush);
  painter.drawRect(cr);

  optional<cpu_util_t> last_util;

  const auto &ticks = _session->get_ticks();
  int transform = -1;
  for(const auto &tick : ticks) {
    ++transform;
    if(!tick) continue;

    int px_trans = transform * (cr.width()-1) / (ticks.size() - 1);
    int px_trans_last = (transform-1) * (cr.width()-1) / (ticks.size() - 1);

    if(last_util) {
      cpu_util_t last = *last_util;
      cpu_util_t cur = tick->cpu_util;

      long double last_field, cur_field;
      long double last_base=0, cur_base=0;

      QPointF points[4];
      QBrush section_brush;
      QPen section_pen;

#define DRAW_GROUP(field, color) \
      FETCH_FIELD(last, _core, field, last_field); \
      FETCH_FIELD(cur, _core, field, cur_field); \
      last_field += last_base; \
      cur_field += cur_base; \
      \
      section_brush = QBrush(QColor("#33" color)); \
      section_pen = QPen(QColor("#" color)); \
      section_pen.setWidth(LINE_STROKE); \
      painter.setPen(section_pen); \
      painter.setBrush(section_brush); \
      points[0] = QPointF(px_trans_last, (1-last_field) * cr.height()); \
      points[1] = QPointF(px_trans, (1-cur_field) * cr.height()); \
      points[2] = QPointF(px_trans, (1-cur_base) * cr.height()); \
      points[3] = QPointF(px_trans_last, (1-last_base) * cr.height()); \
      \
      painter.drawLine(points[0], points[1]); \
      painter.setPen(Qt::NoPen); \
      painter.drawPolygon(points, 4); \
      last_base = last_field; \
      cur_base = cur_field;

      DRAW_GROUP(user, "268bd2");
      DRAW_GROUP(nice, "2aa198");
      DRAW_GROUP(system, "b58900");
      DRAW_GROUP(idle, "859900");
      DRAW_GROUP(iowait, "dc322f");
      DRAW_GROUP(irq, "d33682");
      DRAW_GROUP(softirq, "6c71c4");
    }

    last_util = { tick->cpu_util };
  }
}

CPULegend::CPULegend(QWidget *parent) : QWidget(parent) {
  auto layout = new QBoxLayout(QBoxLayout::LeftToRight);

  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(5);
  layout->setAlignment(Qt::AlignCenter);

  // Centering
  layout->addStretch(1);

  QWidget *box;
  QLabel *label;
  QPalette palette;
  QFont font;

#define ADD_LEGEND(name, color) \
  box = new QWidget(this);\
  box->setFixedSize(10, 10);\
  palette = box->palette();\
  palette.setColor(box->backgroundRole(), "#" color);\
  box->setPalette(palette);\
  box->setAutoFillBackground(true);\
  layout->addWidget(box);\
  \
  label = new QLabel(this);\
  label->setText(#name);\
  font = label->font(); \
  font.setPixelSize(12); \
  label->setFont(font); \
  layout->addWidget(label);\
  layout->addSpacing(10); \

  layout->addSpacing(10);

  ADD_LEGEND(user, "268bd2");
  ADD_LEGEND(nice, "2aa198");
  ADD_LEGEND(system, "b58900");
  ADD_LEGEND(idle, "859900");
  ADD_LEGEND(iowait, "dc322f");
  ADD_LEGEND(irq, "d33682");
  ADD_LEGEND(softirq, "6c71c4");

  // Centering
  layout->addStretch(1);

  this->setLayout(layout);
}
