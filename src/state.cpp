#include "state.h"

using namespace TDB;
using namespace std;

void State::start_timer() {
  // TODO: impl
}

void State::open_local_session() {
  unique_ptr<LocalExecutor> exec(new LocalExecutor);
  this->sessions.emplace_back(new Session(std::move(exec)));
}
