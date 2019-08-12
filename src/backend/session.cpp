#include "backend/session.h"

using namespace TDB;
using namespace std;

Session::Session(std::unique_ptr<Executor> exec) : _aggr(new Aggregator(std::move(exec))) {}
