#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <functional>

using namespace std;

template <typename T>
class RecvBufQueue {
 public:
  RecvBufQueue(const function<void(void)>& notify) : _mutex(), _bufQueue(), _notify(notify) {}
  
  inline bool isEmpty() const {
    lock_guard<mutex> lock(_mutex);
    return _bufQueue.empty();
  }

  inline void enqueue(const shared_ptr<T>& data) {
    {
      lock_guard<mutex> lock(_mutex);
      _bufQueue.push(data);
    }
    /* notify dosn't need to obtain mutex */
    _notify();
  }

  inline shared_ptr<T> dequeue() {
    lock_guard<mutex> lock(_mutex);
    return _bufQueue.pop();
  }
  
 private:
  queue<shared_ptr<T>> _bufQueue;
  mutable mutex _mutex;
  function<void(void)> _notify;
};

