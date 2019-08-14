#include "backend/session.h"

using namespace TDB;
using namespace std;

Session::Session(std::unique_ptr<Executor> exec, int size) : _aggr(new Aggregator(std::move(exec))) {
  ticks.resize(size);
}

void Session::update() {
  tick_t tick = _aggr->tick();

  core_count = tick.cpu_util.cores.size();
  if_count = tick.network_util.ifs.size();

  ticks.push_back(std::move(tick));
  ticks.pop_front();
}

const std::deque<std::optional<tick_t>>& Session::get_ticks() const {
  return ticks;
}

int Session::count_cores() const {
  return core_count;
}
int Session::count_ifs() const {
  return if_count;
}
