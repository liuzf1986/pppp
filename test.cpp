#include "InetSock.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  InetSock sock = InetSock(SOC_TCP);

  std::cout << sock.fd() << std::endl;
  
  return 0;
}


















