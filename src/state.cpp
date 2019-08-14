#include "state.h"

#include <iostream>

using namespace TDB;
using namespace std;

void State::update() {
  for(auto &sess : this->sessions)
    sess->update();
}

void State::open_local_session() {
  unique_ptr<LocalExecutor> exec(new LocalExecutor);
  this->sessions.emplace_back(new Session(std::move(exec), _default_size));

  if(this->active == -1) this->active = 0;
}

void State::open_ssh_session(std::string host, std::string user, int port) {
  unique_ptr<SSHExecutor> exec(new SSHExecutor(host, user, port));
  this->sessions.emplace_back(new Session(std::move(exec), _default_size));

  if(this->active == -1) this->active = 0;
}

std::vector<std::shared_ptr<Session>>& State::get_sessions() {
  return sessions;
}

void State::set_default_size(int size) {
  this->_default_size = size;
}

void State::activate(int i) {
  this->active = i;
}

std::shared_ptr<Session> State::active_session() {
  return sessions[active];
}
