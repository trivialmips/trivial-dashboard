#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "state.h"

#include <memory>
#include <iostream>
#include <string>

#include <QWidget>
#include <QLayout>
#include <QPen>
#include <QBrush>
#include <QPainter>

#define LINE_STROKE 2

namespace TDB::Widgets {
  using NetworkAccessor = std::function<uint64_t(network_util_t)>;
  class NetworkGraph : public QWidget {
    Q_OBJECT

    public:
      NetworkGraph(QWidget *parent, const std::shared_ptr<Session> session, NetworkAccessor acc, std::string color) : _acc(acc), _color(color), _session(session) {};

    protected:
      void paintEvent(QPaintEvent *e) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect cr = contentsRect();

        QBrush bgbrush(QColor("#073642"));

        painter.setPen(Qt::NoPen);
        painter.setBrush(bgbrush);
        painter.drawRect(cr);

        std::vector<std::optional<uint64_t>> converted;

        const auto &ticks = _session->get_ticks();
        converted.reserve(ticks.size());
        uint64_t max_tick = 0;
        for(const auto &tick : ticks) {
          if(!tick)
            converted.push_back({});
          else {
            const auto cur = _acc(tick->network_util);
            if(cur > max_tick) max_tick = cur;
            converted.emplace_back(cur);
          }
        }
        std::cout<<max_tick<<std::endl;

        // Compute scale
        uint64_t scale = 125; // 1Kbps
        while(scale < max_tick) scale *= 10;

        uint64_t scale_number = scale / 125;
        uint64_t scale_unit = 0;
        while(scale_number >= 1000) {
          scale_number /= 1000;
          scale_unit += 1;
        }
        std::string scale_hint = std::to_string(scale_number);
        if(scale_unit == 0) scale_hint += "Kbps";
        else if(scale_unit == 1) scale_hint += "Mbps";
        else if(scale_unit == 2) scale_hint += "Gbps";
        else if(scale_unit == 3) scale_hint += "Tbps";
        // Ok I guess we won't have Pbps

        std::optional<uint64_t> last_opt;

        int transform = -1;
        for(const auto &tick : converted) {
          ++transform;
          if(!tick) continue;

          int px_trans = transform * (cr.width()-1) / (ticks.size() - 1);
          int px_trans_last = (transform-1) * (cr.width()-1) / (ticks.size() - 1);

          if(last_opt) {
            uint64_t last = *last_opt;
            uint64_t cur = *tick;

            long double last_field, cur_field;

            QPointF points[4] = {
              QPointF(px_trans_last, (1-((long double) last) / scale) * cr.height()),
              QPointF(px_trans, (1-((long double) cur) / scale) * cr.height()),
              QPointF(px_trans, cr.height()),
              QPointF(px_trans_last, cr.height()),
            };

            auto brush_color = "#33" + _color;
            auto pen_color = "#" + _color;
            auto brush = QBrush(QColor(brush_color.c_str()));
            auto pen = QPen(QColor(pen_color.c_str()));

            pen.setWidth(LINE_STROKE);
            painter.setPen(pen);
            painter.setBrush(brush);

            painter.drawLine(points[0], points[1]);
            painter.setPen(Qt::NoPen);
            painter.drawPolygon(points, 4);
          }

          last_opt = tick;
        }

        // Draw scale
        std::cout<<"SCALE: "<<scale_hint<<std::endl;
        QPen text_pen(QColor("#839496"));
        painter.setPen(text_pen);
        QFont font = painter.font();
        font.setPixelSize(12);
        painter.setFont(font);
        painter.drawLine(0, 0, 10, 0);
        painter.drawText(cr.adjusted(12, 0, 0, 0), Qt::AlignTop | Qt::AlignLeft, scale_hint.c_str());
      }

    private:
      const std::shared_ptr<Session> _session;
      NetworkAccessor _acc;
      std::string _color;
  };

  class Network : public QWidget {
    Q_OBJECT

    public:
      Network(QWidget *parent, const std::shared_ptr<Session> session, std::string name = "");

    public slots:
      void update();

    private:
      const std::shared_ptr<Session> _session;
      std::string _name;

      NetworkGraph *rx, *tx;
  };
}
#endif // __NETWORK_H__
