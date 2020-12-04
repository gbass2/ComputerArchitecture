#include "ports.hh"
#include <iostream>
// connects master port to a specific slave port
void SlavePort::bind(MasterPort *_mport) {
     mport = _mport;
     if (mport->getSlave() != this)
          mport->bind(this);
}
// disconnects master port from slave port
void SlavePort::unbind() {
     mport = NULL;
}
// passing messages via packets
void SlavePort::sendResp(PacketPtr pkt) {
     pkt->popHeader();             // Pop off because we have already routed back
                                   // to that master port
     mport->recvResp(pkt);         // purely virtual
}

// connects master port to a specific slave port
void MasterPort::bind(SlavePort *_sport) {
     sport = _sport;
     if (sport->getMaster() != this)
          sport->bind(this);
}

// disconnects master port from slave port
void MasterPort::unbind(){
     sport->unbind();
     sport = NULL;
}

void MasterPort::sendReq(PacketPtr pkt){
     pkt->appendHeader(this);      // Appending MasterPorts ID to header
     pkt->printHeader();
     sport->recvReq(pkt);          // purely virtual
}
