#include "backend/executor.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <string_view>

using namespace TDB;
using namespace std;

#define LOCAL_BUFFER_SIZE 128
#define SSH_BUFFER_SIZE 128

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

SSHExecutor::SSHExecutor(string host, string user, int port) {
  _ssh = ssh_new();

  int verbosity = SSH_LOG_PROTOCOL;
  ssh_options_set(_ssh, SSH_OPTIONS_HOST, host.c_str());
  ssh_options_set(_ssh, SSH_OPTIONS_USER, user.c_str());
  ssh_options_set(_ssh, SSH_OPTIONS_PORT, &port);
  ssh_options_set(_ssh, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

  int status = ssh_connect(_ssh);
  if(status != SSH_OK)
    throw std::runtime_error("Unable to connect to host: " + host + ":" + std::to_string(port));

  // TODO: authenticate the server
 
  // Use pubkey auth
  status = ssh_userauth_publickey_auto(_ssh, NULL, NULL);

  if(status == SSH_AUTH_ERROR)
    throw std::runtime_error("Unable to authenticate user: " + user);
}

SSHExecutor::~SSHExecutor() {
  ssh_disconnect(_ssh);
  ssh_free(_ssh);
}

string SSHExecutor::exec(string cmd) {
  cout<<"EXEC"<<endl;
  ssh_channel chan = ssh_channel_new(_ssh);
  if(chan == NULL)
    throw std::runtime_error("Unable to create SSH channel");

  int status = ssh_channel_open_session(chan);
  if(status != SSH_OK) {
    ssh_channel_free(chan);
    throw std::runtime_error("Unable to open SSH channel session");
  }

  status = ssh_channel_request_exec(chan, cmd.c_str());
  if(status != SSH_OK) {
    ssh_channel_close(chan);
    ssh_channel_free(chan);
    throw std::runtime_error("Unable to execute SSH command: " + cmd);
  }

  array<char, SSH_BUFFER_SIZE> buffer;
  string result;

  while(true) {
    int bytes = ssh_channel_read(chan, buffer.data(), buffer.size(), 0);
    if(ssh_channel_is_eof(chan)) break;
    if(bytes < 0) {
      ssh_channel_close(chan);
      ssh_channel_free(chan);
      throw std::runtime_error("SHH transfer error");
    }

    result += string_view(buffer.data(), bytes);
  }

  ssh_channel_send_eof(chan);
  ssh_channel_close(chan);
  ssh_channel_free(chan);

  cout<<"READ:"<<endl;
  cout<<result<<endl;

  return result;
}
