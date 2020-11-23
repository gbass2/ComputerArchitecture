#include "membus.hh"
#include <assert.h>
#include <iostream>
#include <memory>

bool AddressInRange(Addr ad, AddrRange ar) {
     return (ad >= ar.first) && (ad <= ar.second);
}

Membus::Membus(std::shared_ptr<System> sys, Tick forward_time) :
SimObject(sys),
fwd_time(forward_time),
fwd_event(new MembusForwardEvent(this)) {
     memSidePorts.push_back(new MemSidePort(this));
     cpuSidePorts.push_back(new CPUSidePort(this));
}

void Membus::tryToSend() {    // function to try to send to memory
     auto it = packetsWaitingForMemPorts.begin();
     while (it != packetsWaitingForMemPorts.end()) {
          PacketPtr pkt = *it;
          MemSidePort * mem_port = getRequestPort(pkt);
          if (!(mem_port->isBusy())) {  // first in first out queue
               std::cout << "Membus sending packet on Tick: " << currTick() << std::endl;
               mem_port->sendReq(pkt);
               it = packetsWaitingForMemPorts.erase(it);
          }
          else
               it++;
     }
}

void Membus::forwardPackets() {    // a delay in the membus for processing
     Tick curr = currTick();
     // any packet that is in the waiting queue that is eligable to be sent to memory
     // is sent to the queue
     while(!(packetsWaitingForForward.empty()) && (packetsWaitingForForward.front().first = curr)) {
          fwdQType tmp = packetsWaitingForForward.front();
          packetsWaitingForForward.pop_front();
          packetsWaitingForMemPorts.push_back(tmp.second);
     }
     if (!(packetsWaitingForForward.empty())) {
          fwdQType tmp = packetsWaitingForForward.front();
          schedule(fwd_event, tmp.first);
     }

     tryToSend();
}

void Membus::recvReq(PacketPtr pkt) {
     Tick forwardTick = currTick() + fwd_time;
     // queue waiting to be forwarded through the device
     packetsWaitingForForward.push_back(fwdQType(forwardTick, pkt));
     // schedule when eligable to be forwarded to memory
     if (!fwd_event->isScheduled())
          schedule(fwd_event, forwardTick);
     tryToSend();
}

void Membus::recvResp(PacketPtr pkt) {
     CPUSidePort * resp_port = getResponsePort(pkt);
     resp_port->sendResp(pkt);
     // Anytime a response is recieved, try to send again
     // garuntees the ability to always forward on the first cycle that becomes
     // available
     tryToSend();
}

Membus::MemSidePort *
Membus::getRequestPort(PacketPtr pkt) {
     for (auto port : memSidePorts) {
          // check addresses of packet, if it is a match return the port
          if(AddressInRange(pkt->getAddr(), port->getAddrRange()))
               return port;
     }
     std::cout << "Couldn't find the destination port for the packet";
     assert(0);
     return NULL;
}

Membus::CPUSidePort *
Membus::getResponsePort(PacketPtr pkt) {
     for (auto port : cpuSidePorts) {
          // check for the last master port it passed through
          if (port->getMaster() == pkt->getHeaderEnd())
               return port;
     }
     std::cout << "Couldn't find the original port for the packet";
     assert(0);
     return NULL;
}

// Helper functions below for connecting devices onto the Membus

Membus::MemSidePort *
Membus::getMemSidePort(size_t index) {
     if (index < memSidePorts.size())
          return memSidePorts.at(index);
     std::cout << "Couldn't find valid port at index: " << index << std::endl;
     assert(0);
     return NULL;
}

Membus::CPUSidePort *
Membus::getCPUSidePort(size_t index) {
     if (index < cpuSidePorts.size())
          return cpuSidePorts.at(index);
     std::cout << "Couldn't find valid port at index: " << index << std::endl;
     assert(0);
     return NULL;
}

Membus::MemSidePort *
Membus::getUnboundMemSidePort() {
     if (memSidePorts.back()->isBound())
          memSidePorts.push_back(new MemSidePort(this));
     return memSidePorts.back();
}

Membus::CPUSidePort *
Membus::getUnboundCPUSidePort() {
     if (memSidePorts.back()->isBound())
          cpuSidePorts.push_back(new CPUSidePort(this));
     return cpuSidePorts.back();
}
