#include "backend/session.h"

#define TICK_SIZE 20

using namespace TDB;
using namespace std;

Session::Session(std::unique_ptr<Executor> exec) : _aggr(new Aggregator(std::move(exec))) {
  ticks.resize(TICK_SIZE);
}

void Session::update() {
  ticks.push_back(_aggr->tick());
  ticks.pop_front();
}

const std::deque<std::optional<tick_t>>& Session::get_ticks() const {
  return ticks;
}
