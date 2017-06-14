#include <tasks.hpp>

using namespace Tasks;

//first task class with two alternating states and sleep
class T: public TaskCRTP<T>
{
  public:
    T(): TaskCRTP(&T::first) {}
    
    void first()
    {
      Serial.println("FIRST!");
      nextState = &T::second;
      sleep(10);
    }

    void second()
    {
      Serial.println("SECOND!");
      nextState = &T::first;
      sleep(10);
    }
    
    ~T() = default;
};

//a simple task class that only runs run()
class S: public Task
{
  public:
    S() = default;
    
    virtual void run()
    {
      Serial.println("R!");
      sleep(10);
    }
    
    ~S() = default;
};

T t;
S s;

//keep the two task pointers in this simple array
Task* tasks[] = {&t, &s};

//call periodically to update sleep timers
void onTick()
{
  updateSleep(tasks);
}

void setup() {
}

void loop() {
  //here tasks will be run, put to sleep, woken up...
  schedule(tasks);
}
