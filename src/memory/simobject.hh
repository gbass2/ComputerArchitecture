#ifndef SIMOBJECT_H
#define SIMOBJECT_H
#define Tick uint64_t

#include <vector>
#include <deque>
#include <iostream>
#include <string>
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
class RunSim : public Event, SimObject{
private:
    size_t cycles = 0; // Cpu cycle count
public:
    RunSim(std::shared_ptr<System> s) :  Event(), SimObject(s) {} // Calls the CPU constructor so that it will have the same values as the one in main
    void runSimulation(); // Runs the simulation

    virtual void process() override {}
    virtual const char* description() override {return "Setup Simulation";}
    virtual void initialize() override { // Initialzes MEQ with a fetch event
        std::cout << "scheduling Setup Simulation on Tick " << currTick() << std::endl;
        sysMain->schedule(this, 0); // Scheduling new event
    }
};

#endif //SIMOBJECT_H
