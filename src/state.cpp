#include "state.h"

#include <iostream>

using namespace TDB;
using namespace std;

void State::update() {
  cout<<"UPDATE"<<endl;
  for(auto &sess : this->sessions)
    sess->update();
}

void State::open_local_session() {
  unique_ptr<LocalExecutor> exec(new LocalExecutor);
  this->sessions.emplace_back(new Session(std::move(exec), _default_size));
}

void State::open_ssh_session(std::string host, std::string user, int port) {
  unique_ptr<SSHExecutor> exec(new SSHExecutor(host, user, port));
  this->sessions.emplace_back(new Session(std::move(exec), _default_size));
}

std::vector<std::shared_ptr<Session>>& State::get_sessions() {
  return sessions;
}

void State::set_default_size(int size) {
  this->_default_size = size;
}
