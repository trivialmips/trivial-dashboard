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
  this->sessions.emplace_back(new Session(std::move(exec)));
}

std::vector<std::shared_ptr<Session>>& State::get_sessions() {
  return sessions;
}
