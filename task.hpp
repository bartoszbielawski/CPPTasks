#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>

#define TASK(name) class name: public Tasks::TaskCTRP<name>

namespace Tasks
{
  enum class State
  {
    READY,
    SLEEPING,
    SUSPENDED,
    DEAD
  };

  class Task
  {
    public:
      Task(): _state(State::READY), _timer(0) {}
      virtual ~Task() {}

      Task& operator=(Task& other) = delete;
      Task(Task& other) = delete;

      virtual void run() = 0;

      State getState() const {return _state;}

      void resume() {_state = State::READY;}
      void suspend() {_state = State::SUSPENDED;}
      void sleep(uint32_t ticks) {_timer = ticks; _state = State::SLEEPING;}
      void kill() {_timer = 0; _state = State::DEAD;}

      uint32_t decreaseTimer() {return --_timer;}

    private:
      State    _state;
      uint32_t _timer;
  };

  template <class T>
  class TaskCTRP: public Task
  {
    public:
      typedef void (T::*state)();

      TaskCTRP(state s):  nextState(s) {}
      virtual ~TaskCTRP() {}

      virtual void run()
      {
        (static_cast<T*>(this)->*nextState)();
      }

    protected:
      state nextState;
  };


  template <class T>
  void schedule(const T& container)
  {
      for (auto& e: container)
      {
        if (e == nullptr)
          continue;

        if (e->getState() == State::READY)
          e->run();
      }
  }

  template <class T>
  void removeDead(T& container)
  {
    container.erase(std::remove_if(std::begin(container), std::end(container), [](typename T::value_type& v) {
      auto s = v->getState();
      return s == State::DEAD;
    }), container.end());
  }

  template <class T>
  void updateSleep(const T& container)
  {
    for (auto& e: container)
    {
      if (e->getState() != State::SLEEPING)
        continue;
      uint32_t t = e->decreaseTimer();
      if (!t)
        e->resume();
    }
  }

}
