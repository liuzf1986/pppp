
#include <iostream>
#include "InetSock.hpp"
#include "UdpIOExec.hpp"
#include <memory>
#include "RecvBufQueue.hpp"
#include "ProcessPoll.hpp"

using namespace std;

void on_readable(void* data, size_t len, struct InetPeerUdp* peer) {
  printf("on_readble === \n");
  printf("%s \n", data);
}

void on_error(int errno) {
  
}

void notify() {
  printf("%s xxxxxxxxxxx \n", __FUNCTION__);
}

int g_proc_count = 0;

struct Base {
  static PROCESS_RESULT doTask() {
    if(g_proc_count < 10) {
      printf("qqqqqqqqqqqqq %d\n", g_proc_count);
      g_proc_count ++;
      return RESULT_OK;
    } else {
      return RESULT_DONE;
    }
  }
};

int main(int argc, char *argv[])
{
  int ret;
  InetSock sock = InetSock(SOC_UDP);
  ret = sock.bind(3002);
  assert(!ret);

  ProcessPoll<Base> p(5);

  for(int i = 0; i < 25; i ++) {
    usleep(500);
    p.wakeup();    
  }
  
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














