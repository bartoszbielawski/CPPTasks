
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

class printTask: public TaskCTRP<printTask>
{
  public:
    printTask(const string& name): TaskCTRP(&printTask::state1), name(name)  {}

    void state1()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state2;
      this->sleep(2);
    }

    void state2()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state1;
      this->sleep(2);
    }

    virtual ~printTask() {cout << "~" << name << endl;}

  private:
    string name;
};

class boringTask: public TaskCTRP<boringTask>
{
  public:
    boringTask(): TaskCTRP(&boringTask::state1) {}
  private:
    void state1() {cout << "Boooring..."<< endl; sleep(1);}
};

TASK(suicidal)
{
  public:
    suicidal(): TaskCTRP(&suicidal::state1) {}
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

void timerISR()
{
  while (run)
  {
    updateSleep(tasks);
    sleep(1);
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

  cout << sizeof(Tasks::Task) << endl;
  cout << sizeof(Tasks::TaskCTRP<boringTask>) << endl;
  cout << sizeof(boringTask) << endl;

  tasks.emplace_back(new printTask("1"));
  tasks.emplace_back(new boringTask());
  tasks.emplace_back(new suicidal());

  while (run)
  {
    schedule(tasks);
    removeDead(tasks);
  }

  s.join();
  t.join();
}
