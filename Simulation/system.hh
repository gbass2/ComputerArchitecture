#ifndef SYSTEM_H
#define SYSTEM_H

#include <deque>
#include <cstdint>
#include "event.hh"

#define Tick uint64_t

class System{
private:
    Tick currentTick; // Holds the current run time
    std::deque<Event *> MEQ; // Master Event Queue
    std::deque<Event *>::iterator findEvent(Event *e); // Finds an event in the MEQ
public:
    System() : currentTick(1) {}
    Tick currTick(){ return currentTick; }
    void incTick(Tick t) { currentTick += t; }
    void schedule(Event *e, Tick t); // Schedule and event in the MEQ. Throw and error if event already scheduled
    void reschedule(Event *e, Tick t); // Reschedule an event or schedule if not found
    std::deque<Event *> getMEQ() { return MEQ; }
    void printMEQ();
    void removeEvent(); // Removes an event from the MEQ
    void clearMEQ() { MEQ.clear(); }  // Clears the entire MEQ
};

#endif // SYSTEM_H
