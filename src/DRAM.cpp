#include "DRAM.hh"
#include <assert.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <memory>

DRAM::DRAM(std::shared_ptr<System> sys, AddrRange _addrs, Tick respLatency) :
     SimObject(sys),
     addrs(_addrs),
     responceLatency(respLatency),
     port(new MemPort(this)),
     e(new DRAMEvent(this)) {
          size = addrs.second - addrs.first + 1;
          memory = (uint8_t*)std::malloc(size);
     }

void DRAM::process(){
     // handles the response
     std::cout << "DRAM is responding to a request on Tick:" << currTick() << std::endl;
     port->sendResp(request); // respond to packet
}

void DRAM::recvReq(PacketPtr pkt) {
     std::cout << "DRAM received a request on Tick:" << currTick() << std::endl;
     if (pkt->isRead())
          // get data and load it into the packet
          getDataAtAddr(pkt->getAddr(), pkt->getBuffer(), pkt->getSize());
     else
          // get data from packet and store it in memory
          setDataAddr(pkt->getAddr(), pkt->getBuffer(), pkt->getSize());
     request = pkt; // store active request so we can retrieve it later
     schedule(e, currTick() + responceLatency);   // schedule response event
}

// In the case that we have a packet, the packet will have a buffer associated
// with it and size
void DRAM::setDataAddr(Addr ad, uint8_t * buff, size_t len){
     // assert((ad >= addrs.first) && ((ad+len) <= addrs.second + 1));
     Addr offset = ad - addrs.first;    // Calcs offest in memory that the
                                        // packet is reading

     std::memcpy((memory + offset), buff, len);   // pointer in our memory for DRAM
}
// Helper Functions
template<typename T>     // allows the function to use multiple different types
void DRAM::writeAtAddr(Addr ad, T val) {
     setDataAddr(ad, (uint8_t *)(&val), sizeof(T));
}

void DRAM::writeByteAtAddr(Addr ad, uint8_t val) {     // T type = byte
     writeAtAddr<uint8_t>(ad, val);
}

void DRAM::writeHalfwordAtAddr(Addr ad, uint16_t val) {// T type = short
     writeAtAddr<uint16_t>(ad, val);
}

void DRAM::writeWordAtAddr(Addr ad, uint32_t val) {    // T type unsigned int
     writeAtAddr<uint32_t>(ad, val);
}

void DRAM::writeDoubleWordAtAddr(Addr ad, uint64_t val) {   // T type long
     writeAtAddr<uint64_t>(ad, val);
}

void DRAM::getDataAtAddr(Addr ad, uint8_t * buff, size_t len) {
    assert((ad >= addrs.first) && ((ad+len) <= addrs.second + 1));
    Addr offset = ad - addrs.first;

    std::memcpy(buff, (memory + offset), len);
}

template<typename T>
T
DRAM::readAtAddr(Addr ad) {
     T val;
     getDataAtAddr(ad, (uint8_t*)(&val), sizeof(T));
     return val;
}
// Helper Functions
uint8_t
DRAM::readByteAtAddr(Addr ad) {
     return readAtAddr<uint8_t>(ad);
}

uint16_t
DRAM::readHalfwordAtAddr(Addr ad) {
     return readAtAddr<uint16_t>(ad);
}

uint32_t
DRAM::readWordAtAddr(Addr ad) {
     return readAtAddr<uint32_t>(ad);
}

uint64_t
DRAM::readDoubleWordAtAddr(Addr ad) {
     return readAtAddr<uint64_t>(ad);
}
