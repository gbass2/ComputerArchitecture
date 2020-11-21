#include <iostream>
// #include "memrequester.hh"
#include "DRAM.hh"
#include "membus.hh"

int main(){
     System *sys = new System();
     // two devices making memory requests
     // arbitrator between them because they are trying to access the same memory
     // device at the same time
     memrequester * dev0 = new memrequester(sys, "dev0", 0, 1023);
     memrequester * dev1 = new memrequester(sys, "dev1", 0, 1023);
     // stores a 1KB array
     DRAM * ram = new DRAM(sys, AddrRange(0,1023), 20);     // 20 = response time
     Membus * bus = new Membus(sys, 0);      // 5 = forwarding time

     // dev0->getPort()->bind(ram->getPort());

     // binding the devices port to the bus
     dev0->getPort()->bind(bus->getUnboundCPUSidePort());
     dev1->getPort()->bind(bus->getUnboundCPUSidePort());
     ram->getPort()->bind(bus->getUnboundCPUSidePort());

     // fill DRAM with random floating point values
     for (auto i = ram->getAddrRange().first; i < ram->getAddrRange().second; i+=4){
          float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
          uint32_t val = *(uint32_t *)(&r);
          ram->writeWordAtAddr(i, val);
     }

     // schedule first events
     dev0->initialize();
     dev1->initialize();
     sys->runSimulation();
     return 0;
}
