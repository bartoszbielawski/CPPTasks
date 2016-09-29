
#include "task.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <thread>

using namespace std;
using namespace Task;

class printTask: public Task::TaskCTRP<printTask>
{
  public:
    printTask(const string& name): TaskCTRP(&printTask::state1), name(name)  {}

    void state1()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state2;
      this->sleep(1);
    }

    void state2()
    {
      cout << __PRETTY_FUNCTION__ << " " << name << endl;
      nextState = &printTask::state1;
      this->sleep(1);
    }

    virtual ~printTask() {cout << "~" << name << endl;}

  private:
    string name;
};


vector<unique_ptr<Task::Task>> tasks;
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

  tasks.emplace_back(new printTask("1"));
  tasks.emplace_back(new printTask("2"));

  while (run)
  {
    schedule(tasks);
  }

  s.join();
  t.join();
}
