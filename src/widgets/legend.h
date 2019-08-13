#ifndef __LEGEND_H__
#define __LEGEND_H__

#include <QWidget>

namespace TDB::Widgets {
  class Legend : public QWidget {
      Q_OBJECT

      public:
        Legend(QWidget *parent, std::vector<std::pair<std::string, std::string>> legends);
  };
}

#endif // __LEGEND_H__
