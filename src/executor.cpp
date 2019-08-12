#include "executor.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

using namespace TDB;
using namespace std;

#define LOCAL_BUFFER_SIZE 128

string LocalExecutor::exec(string cmd) {
  array<char, LOCAL_BUFFER_SIZE> buffer;
  string result;

  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

  if(!pipe)
    throw std::runtime_error("Unable to execute local command: " + cmd);

  while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    result += buffer.data();

  return result;
}
