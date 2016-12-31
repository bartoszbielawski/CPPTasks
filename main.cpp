
#include "task.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <thread>
#include <array>
#include <list>

using namespace std;
using namespace Tasks;

class printTask: public TaskCRTP<printTask>
{
  public:
    printTask(const string& name): TaskCRTP(&printTask::state1), name(name)  {}

    void state1()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state2;
      sleep(2);
    }

    void state2()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state1;
      sleep(2);
    }

    virtual ~printTask() {cout << "~" << name << endl;}

  private:
    string name;
};

class boringTask: public TaskCRTP<boringTask>
{
  public:
    boringTask(): TaskCRTP(&boringTask::state1) {}
  private:
    void state1() {cout << "Boooring..."<< endl; sleep(1);}
};

class suicidal: public TaskCRTP<suicidal>
{
  public:
    suicidal(): TaskCRTP(&suicidal::state1) {}
  private:
    void state1()
    {
      cout << "TimeLeft: " << timeLeft << endl;
      if (--timeLeft <= 0)
        kill();
      else
        sleep(1);
    }

    int32_t timeLeft = 5;
};



list<unique_ptr<Task>> tasks;

volatile bool run = true;

volatile uint32_t timer = 0;

class WakeUpAtFive: public TaskCRTP<WakeUpAtFive, Tasks::SimpleCond>
{
public:
  WakeUpAtFive(): TaskCRTP(&WakeUpAtFive::state1)
  {
    wait(60, [](){return timer >= 5;});
  }

  void state1()
  {
    cout << "Woken up! " << (timedOut() ? "Timed out...": "Condition met!") << endl;
    kill();
  }
};


void timerISR()
{
  while (run)
  {
    updateSleep(tasks);
    sleep(1);
    timer++;
  }
}

void stop()
{
  sleep(10);
  run = false;
}

int main()
{
  std::thread t(timerISR);
  std::thread s(stop);

  cout << "Base class size:   " << sizeof(Tasks::Task) << endl;
  cout << "Empty usable size: " << sizeof(boringTask) << endl;
  cout << "Suicidal size:     " << sizeof(suicidal) << endl;
  cout << "WU@5 size:         " << sizeof(WakeUpAtFive) << endl;

  tasks.emplace_back(new printTask("1"));
  tasks.emplace_back(new boringTask());
  tasks.emplace_back(new suicidal());
  tasks.emplace_back(new WakeUpAtFive());

  while (run)
  {
    schedule(tasks);
    removeDead(tasks);
  }

  s.join();
  t.join();
}
