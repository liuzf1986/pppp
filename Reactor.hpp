#pragma once

#include <thread>

using namespace std;

/**
 * IOExec : such as UdpIOExec, TcpIOExec
 * N : specify how much thread will process readble event
 */
template <class IOExec>
class Reactor {
 public:
  
 private:
  IOExec _executor;
};

