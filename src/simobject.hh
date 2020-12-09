#ifndef SIMOBJECT_H
#define SIMOBJECT_H
#define Tick uint64_t

#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "event.hh"
#include "system.hh"

class SimObject {
protected:
    std::shared_ptr<System> sysMain;
    const char *name;

public:
    Tick currTick() { return sysMain->currTick(); } // Gets the current sysMaintem time
    void incTick(Tick t) { sysMain->incTick(t); } // Increments the sysMaintem time
    void schedule(Event *e, Tick t) { sysMain->schedule(e,t); } // Schedules an event
    void reschedule(Event *e, Tick t) { sysMain->reschedule(e,t); } // Reschedules an event
    void printMEQ() { sysMain->printMEQ(); }
    const char* getName() { return name; }
    virtual void initialize() = 0; // Creates the first event
    SimObject(std::shared_ptr<System> s4, const char *name) : sysMain(s4), name(name) {}
    SimObject(std::shared_ptr<System> s4) : sysMain(s4) {}
};

// Runs the simulation
class RunSim : SimObject{
private:
public:
    double cycles = 0; // Cpu cycle count
    RunSim(std::shared_ptr<System> s) : SimObject(s) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation

    virtual void initialize() override {}
};

#endif //SIMOBJECT_H
