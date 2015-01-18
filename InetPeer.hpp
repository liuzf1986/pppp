#pragma once
#include <netinet/in.h>

struct InetPeerTcp {
  inline int sum () const {
    return _fd;
  }
  
  int _fd;
};

struct InetPeerUdp {
  inline int sum () const {
    return _addr.sin_addr.s_addr;
  }
  
  struct sockaddr_in _addr;
};


















