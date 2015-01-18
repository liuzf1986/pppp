#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include <thread>
#include <memory>
#include <cassert>
#include <assert.h>
#include <netinet/in.h>
#include <mutex>

#include "UdpIOExec.hpp"
#include "MemPool.hpp"
#include "InetPeer.hpp"

using namespace std;

template <template <int N> class FixedAlloc=SimpleAllocator, int MTU=1200>
class UdpIOExec {
 public:
  UdpIOExec(int fd, const function<void(void*, size_t, struct InetPeerUdp*)>& on_readable, const function<void(int)>& on_error) :
      _fd(fd),
      _working(true),
      _on_readable(on_readable),
      _on_error(on_error),
      _thread(std::bind(&UdpIOExec::thread_loop, this, fd, MTU))
  {
    assert(nullptr != _on_readable);
  }

  ~UdpIOExec() {
    {
      lock_guard<mutex> lock(_mutex);
      if(_thread.joinable()) {
        _thread.detach();
      }
    }
  }

  void stopSync() {
    lock_guard<mutex> lock(_mutex);
    _working = false;
    if(_thread.joinable()) {
      _thread.join();
    }
  }

  
 private:
  void thread_loop(int fd, size_t mtu) {
    printf("UdpIOExec thread loop executing, fd=%d, mtu=%lu \n", fd, mtu);
    
    while (_working) {
      void* bufptr = _recvbuf_alloc.alloc(1);
      assert(nullptr != bufptr);

      InetPeerUdp* udpPeer = static_cast<InetPeerUdp*>(_peer_alloc.alloc(1));
      assert(nullptr != udpPeer);

      socklen_t addrLen = sizeof(udpPeer->_addr);
      
      ssize_t recvLen = recvfrom(fd, bufptr, mtu, 0, (struct sockaddr *)&udpPeer->_addr, &addrLen);

      if(__builtin_expect(recvLen > 0, 1)) {
        _on_readable(bufptr, recvLen, udpPeer);
      } else if(EAGAIN == recvLen || EWOULDBLOCK == recvLen) {
        printf("UdpIOExec recv a wouldblock msg");
        continue;
      } else {
        if(nullptr != _on_error) {
          printf("UdpIOExec recv error=%ld", recvLen);
          _on_error(recvLen);
        }
      }
    }
  }
  thread _thread;
  mutex _mutex;
  int _fd;
  bool _working;
  function<void(void*, size_t, struct InetPeerUdp*)> _on_readable;
  function<void(int)> _on_error;

  FixedAlloc<MTU> _recvbuf_alloc;
  FixedAlloc<sizeof(InetPeerUdp)> _peer_alloc;  
};














