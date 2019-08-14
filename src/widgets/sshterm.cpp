#include "sshterm.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace TDB::Widgets;
using namespace std::chrono_literals;

#define BUF_SIZE 256

SSHTerm::SSHTerm(QWidget *parent, ssh_channel chan, mutex *ssh_mutex) : QTermWidget(0, parent), _chan(chan) {
  connect(this, &SSHTerm::sendData, [this, ssh_mutex](const char *data, int size) {
    cout<<"--- SEND"<<endl;
    cout<<string_view(data, size)<<endl;
    unique_lock lock(*ssh_mutex);
    ssh_channel_write(this->_chan, data, size);
  });

  reader = make_unique<std::thread>([this, ssh_mutex]() {
    cout<<"--- READ"<<endl;
    array<char, BUF_SIZE> buffer;
    while(ssh_channel_is_open(this->_chan) && !ssh_channel_is_eof(this->_chan)) {
      unique_lock lock(*ssh_mutex);
      int bytes = ssh_channel_read_nonblocking(this->_chan, buffer.data(), buffer.size(), 0);
      lock.unlock();
      if(bytes < 0) {
        ssh_channel_close(this->_chan);
        ssh_channel_free(this->_chan);
        throw std::runtime_error("SHH transfer error");
      } else if(bytes > 0) {
        cout<<"READ: "<<string_view(buffer.data(), bytes)<<endl;
        write(this->getPtySlaveFd(), buffer.data(), bytes);
      } else {
        std::this_thread::sleep_for(5us);
      }
    }
  });

  this->startTerminalTeletype();
}
