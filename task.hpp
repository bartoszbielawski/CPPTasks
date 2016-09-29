#pragma once

#include <functional>
#include <cstdint>

namespace Task
{
  enum class State
  {
    READY,
    SLEEPING,
    SUSPENDED
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

        switch (e->getState())
        {
          case State::READY:
            e->run();
            break;
          case State::SLEEPING:
          case State::SUSPENDED:
            break;
        }
      }
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
