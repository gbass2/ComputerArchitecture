#ifndef PORTS_HH
#define PORTS_HH

#include "packet.hh"

class SlavePort{ // Servicer. Recieve a request and generate a responce.
private:
    MasterPort *mport;

public:
    SlavePort() {}
    // Connects a specific master port to a specific slave port. So they can directly interface with each other
    void bind(MasterPort * _mport);
    void unbind();
    bool isBound() { return (mport);}
    MasterPort *getMaster() { return mport; }

    virtual void sendResp(PacketPtr pkt); // Send a responce. Pop off the pointer from the header. We have identified the master port we need to transfer to/go back to.
    virtual void recvReq(PacketPtr pkt) = 0; // Recieve a request. Extend the class for specific examples on our system.
    virtual const AddrRange getAddrRange() = 0; // Sets the memory address range for a particular device. So that multiple devices do not clash
};

class MasterPort{ // Requester
private:
    SlavePort * sport;
public:
    MasterPort() {}
    void bind(SlavePort *_sport);
    void unbind();
    bool isBound() { return (sport); }
    SlavePort *getSlave() { return sport; }

    virtual void sendReq(PacketPtr pkt); // Send a request. Append the pointer to the master to the header
    virtual void recvResp(PacketPtr pkt) = 0; // Recieve a responce. Recieve a request. Extend the class for specific examples on our system.
    const AddrRange getAddrRange() { return sport->getAddrRange(); } // Get the address range of the slave
};

#endif // PORTS_HH
