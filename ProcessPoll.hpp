#pragma once

#include <thread>
#include <forward_list>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

enum PROCESS_RESULT {
  RESULT_OK,
  RESULT_ERR,
  RESULT_DONE
};

template <class PROC>
class ProcessPoll {
 public:
  ProcessPoll(size_t nthreads)
      : _stoped(false), _mutex(), _cond()
  {
    for(int i = 0; i < nthreads; i++) {
      unique_ptr<thread> upThread(new thread(bind(&ProcessPoll::process_loop, this)));
      _threads.push_front(move(upThread));
    }
  }

  ~ProcessPoll() {
    /* wait all thread to done */
    _stoped = true;
    _cond.notify_all();

    for (auto& thd: _threads) {
      if(thd->joinable()) {
        thd->join();
      }
    }
  }

  void wakeup() {
    _cond.notify_one();
  }
  
 private:
  void process_loop() {
    while(!_stoped) {
      unique_lock<mutex> ulock(_mutex);
      _cond.wait(ulock);
      do { printf("enter .... \n");}
      while(RESULT_OK == PROC::doTask());
      _mutex.unlock();
    }
  }
  mutable mutex _mutex;
  condition_variable _cond;
  forward_list<unique_ptr<thread>> _threads;
  bool _stoped;
};


