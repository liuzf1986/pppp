#pragma once
#include <netinet/in.h>

class InetPeerTcp {
 private:
  int _fd;
};

class InetPeerUdp {
  
 private:
  struct sockaddr_in _addr;
};



















