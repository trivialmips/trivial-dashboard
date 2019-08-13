#include "backend/session.h"

using namespace TDB;
using namespace std;

Session::Session(std::unique_ptr<Executor> exec, int size) : _aggr(new Aggregator(std::move(exec))) {
  ticks.resize(size);
}

void Session::update() {
  ticks.push_back(_aggr->tick());
  ticks.pop_front();
}

const std::deque<std::optional<tick_t>>& Session::get_ticks() const {
  return ticks;
}
