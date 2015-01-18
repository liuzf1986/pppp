#pragma once

#include <thread>
#include <stack>
#include <memory>
#include <RecvBufQueue.hpp>

using namespace std;

template <class MSG>
class Dispatchers {
 public:
  Dispatchers(const Dispatchers&) = delete;
  Dispatchers& operator=(const Dispatchers&) = delete;
  
  Dispatchers(size_t nthreads, shared_ptr<RecvBufQueue<MSG> > dataSrc) {
    
  } 
 private:
  void dispatch_loop(shared_ptr<RecvBufQueue<MSG> >) {
    
  }
  
  stack<shared_ptr<thread>> _idleThreads;
  size_t _initCount;
  shared_ptr<RecvBufQueue(const function<void ()> &notify)>
};



















