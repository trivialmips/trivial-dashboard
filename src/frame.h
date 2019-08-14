#ifndef __FRAME_H__
#define __FRAME_H__

#include "state.h"

#include "widgets/cpu.h"
#include "widgets/network.h"
#include "widgets/legend.h"

#include <QMainWindow>
#include <QEvent>
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>

#include <vector>

namespace TDB {
  class Frame : public QMainWindow {
    Q_OBJECT

    public:
      Frame(std::optional<std::tuple<std::string, std::string, int>> remote, int size);
      State state;

    protected:
      bool eventFilter(QObject *obj, QEvent *ev) override;

    public slots:
      void exit();
      void tick();

    private:
      void gotoHomeView();
      void gotoCPUView();
      void gotoNetworkView();

      void updateCoreLabels();
      void updateIfLabels();

      Widgets::CPU *cpu;
      Widgets::Network *network;

      std::vector<Widgets::CPU *> cores;
      std::vector<Widgets::Network *> ifs;
      std::vector<QLabel *> core_labels;
      std::vector<QLabel *> if_labels;
      std::vector<std::string> if_names;

      QBoxLayout *home_layout = nullptr;
      QGridLayout *cpu_layout = nullptr, *network_layout = nullptr;
      QWidget *home_widget, *cpu_widget, *network_widget;
      QStackedWidget *container;
      QLabel *title;
  };
}

#endif // __FRAME_H__
