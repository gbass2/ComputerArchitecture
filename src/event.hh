#ifndef EVENT_H
#define EVENT_H

#include <cstdint>

#define Tick uint64_t

class Event{
private:
    Tick time; // Time the event is going to be executed
    bool scheduled = false; // Boolean for if an event is scheduled or not
    bool priority = false; // Boolean to determine if the event should be pushed to the top op MEQ

public:
    Event() : time(0), scheduled(0), priority(0) {}
    Event(bool priority) : time(0), scheduled(0), priority(priority) {}
    Tick getTime() { return time; }
    void schedule(Tick t){
        time = t;
        scheduled = true;
    }
    void deschedule(){
        time = -1;
        scheduled = false;
    }
    bool isScheduled() { return scheduled; }
    bool getPriority() { return priority; }
    virtual void process() = 0;
    virtual const char* description() = 0;

};

#endif // EVENT_H
