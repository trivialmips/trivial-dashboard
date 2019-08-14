#ifndef __SSHTERM_H__
#define __SSHTERM_H__

#include <qtermwidget5/qtermwidget.h>
#include <libssh/libssh.h>

#include <thread>
#include <memory>
#include <mutex>

namespace TDB::Widgets {
  class SSHTerm : public QTermWidget {
    Q_OBJECT

    public:
      SSHTerm(QWidget *parent, ssh_channel chan, std::mutex *ssh_mutex);
    private:
      ssh_channel _chan;
      std::unique_ptr<std::thread> reader;
  };
}

#endif // __SSHTERM_H__
