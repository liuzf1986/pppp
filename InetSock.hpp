#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

typedef enum {
  SOC_TCP,
  SOC_UDP
} InetSockType;

class InetSock {
 public:
  InetSock(int fd) : _fd(fd) {}
  InetSock(InetSockType type) : _fd(-1) {
    if(SOC_TCP == type) {
      _fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    } else {
      _fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    
    if(-1 == _fd) {
      // fix
    }
  }

  inline int fd() const { return _fd; }

  int bind(uint16_t hostport) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(hostport);

    return ::bind(_fd, (const struct sockaddr*)&addr, static_cast<socklen_t>(sizeof(addr)));
  }
  
  int bind(const struct sockaddr_in& addr) {
    return ::bind(_fd, (const struct sockaddr*)&addr, static_cast<socklen_t>(sizeof(addr)));
  }

  int setReuseAddress(bool enable) {
    int opt = enable ? 1 : 0;
    return ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
  }

  int setReusePort(bool enable) {
    int opt = enable ? 1 : 0;
    return ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
  }

  int listen(int backlog = SOMAXCONN) {
    return ::listen(_fd, backlog);    
  }

  int accept(struct sockaddr_in* addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr));
    return ::accept(_fd, (struct sockaddr*)(addr), &addrlen);
  }

  int connect(const struct sockaddr_in& remote) {
    return ::connect(_fd, (const struct sockaddr*)&remote, static_cast<socklen_t>(sizeof(remote)));
  }

  int setSendBufferSize(int size) {
    int iSize = size;
    return ::setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, &iSize, static_cast<socklen_t>(iSize));
  }

  int setRecvBufferSize(int size) {
    int iSize = size;
    return ::setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, &iSize, static_cast<socklen_t>(iSize));
  }

  int setNonBlocking(bool enable) {
    int flags = fcntl(_fd, F_GETFL, 0);
    
    if(enable) {
      fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
    } else {
      fcntl(_fd, F_SETFL, flags & ~O_NONBLOCK);
    }
  }

  int close() {
    return ::close(_fd);
  }
      
 private:
  int _fd;
};
















