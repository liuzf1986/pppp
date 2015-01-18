
#include <iostream>
#include "InetSock.hpp"
#include "UdpIOExec.hpp"
#include <memory>
#include "RecvBufQueue.hpp"

using namespace std;

void on_readable(void* data, size_t len, struct InetPeerUdp* peer) {
  printf("on_readble === \n");
  printf("%s \n", data);
  //  printf("%s:%d", peer->_addr)
}

void on_error(int errno) {
  
}

void notify() {
  printf("%s xxxxxxxxxxx \n", __FUNCTION__);
}

struct Base {
  
};

int main(int argc, char *argv[])
{
  int ret;
  InetSock sock = InetSock(SOC_UDP);
  ret = sock.bind(3002);
  assert(!ret);

  sock.setNonBlocking(false);
  auto onread = function<void(void*, size_t, struct InetPeerUdp*)>(on_readable);
  UdpIOExec<> exec(sock.fd(), onread, nullptr);

  function<void(void)> func = notify; 

  shared_ptr<Base> bPtr(new Base);
  
  RecvBufQueue<Base> q(func);
  q.enqueue(bPtr);

  pause();
  return 0;
}














