#include "system.hh"
#include <iostream>
#include <cstring>
#include <cassert>

std::deque<Event *>::iterator System::findEvent(Event *e){ // Finds an event in the MEQ
    auto it = MEQ.begin();
    for(it; it != MEQ.end(); it++){
        if(e == *it){
            std::cout << "Event Found" << std::endl;
            return it;
        }
    }
    return it;
}

void System::schedule(Event *e, Tick t){ // Schedules an event into MEQ
    // assert(t <= currTick());

    // Adding the event to MEQ
    if(!(e->isScheduled())){
        e->schedule(t);

        if(e->getPriority()) {
            MEQ.push_front(e);
            return;
        } else{
            for(auto it = MEQ.begin(); it != MEQ.end(); it++){
                if(e->getTime() < (*it)->getTime()){
                    MEQ.insert(it, e);
                    return;
                }
            }
            MEQ.push_back(e);
        }
    } else {
        std::cout << e->description() << std::endl;
        std::cout << MEQ.front()->description() << std::endl;
        assert(0);
    }
}

void System::reschedule(Event *e, Tick t){ // Reschedules an event in MEQ
    assert(t >= currentTick);
    if(e->isScheduled()){

        MEQ.erase(findEvent(e));

        // std::cout << MEQ.front()->description();

        e->schedule(t);

        for(auto it = MEQ.begin(); it != MEQ.end(); it++){  // goes through and figures out where to put it in based on the time you gave it
            if((*it)->getTime() == e->getTime())
                break;

            MEQ.insert(it, e);
            std::cout << "here" << std::endl;
            return;
        }
        MEQ.push_back(e);
    }
}

// Prints the contents in MEQ
void System::printMEQ(){
    std::cout << "MEQ begin" << std::endl;
    for(size_t i = 0; i < MEQ.size(); i++){
        std::cout << (MEQ.at(i))->getTime() << ":" << (MEQ.at(i))->description() << std::endl;
    }
    std::cout << "MEQ end" << std::endl << std::endl;
}

Event *System::popEvent() {
   Event * tmp = MEQ.front();
   tmp->deschedule();
   MEQ.erase(MEQ.begin());
   return tmp;
}

// Removes an Event from MEQ
void System::removeEvent(){
    MEQ.front()->deschedule();
    MEQ.pop_front();
}
