#include "system.hh"
#include <iostream>

std::deque<Event *>::iterator System::findEvent(Event *e){ // Finds an event in the MEQ
    for(auto it = MEQ.begin(); it != MEQ.end(); it++){
        if(e == *it){
            std::cout << "Event Found" << std::endl;
            return it;
        }
    }
}

void System::schedule(Event *e, Tick t){ // Schedules an event into MEQ
    assert(t >= currTick()); // Exits if
    // Adding the event to MEQ
    if(!(e->isScheduled())){
        e->schedule(t);

        for(auto it = MEQ.begin(); it != MEQ.end(); it++){
            if(e->getTime() < (*it)->getTime()){
                MEQ.insert(it, e);
                return;
            }
        }
        MEQ.push_back(e);
    } else {
        assert(0);
    }
}

void System::reschedule(Event *e, Tick t){ // Reschedules an event in MEQ
    assert(t >= currentTick);
    if(e->isScheduled()){
        MEQ.erase(findEvent(e));
        e->schedule(t);

        for(auto it = MEQ.begin(); it != MEQ.end(); it++){
            MEQ.insert(it, e);
            return;
        }
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

// Removes an Event from MEQ
void System::removeEvent(){
    MEQ.front()->deschedule();
    MEQ.pop_front();
}
