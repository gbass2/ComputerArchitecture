#ifndef DRAM_HH
#define DRAM_HH

#include "simobject.hh"
#include "ports.hh"
#include <memory>

// Change to implement bitset

class DRAM : public SimObject{
private:
    AddrRange addrs; // Set of addresses. Starting and ending.
    size_t size; // How large the memory is;
    Tick responceLatency; // How long it takes to respond to a request
    uint8_t *memory; // Storage component

    // MemPort is a type of SlavePort
    class MemPort : public SlavePort{
    private:
        DRAM *owner; // Points back to the dram object that contains this port
    public:
        MemPort(DRAM * _owner) : SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt); }
        const AddrRange getAddrRange() override { return owner->getAddrRange(); }

    };

    // Responce event
    class DRAMEvent : public Event{
    private:
        DRAM *owner;
    public:
        DRAMEvent(DRAM * _owner) : Event(), owner(_owner) {}
        void process() override { owner->process(); }
        const char* description() override { return "DRAM Responce Event"; }
    };

    MemPort *port;
    DRAMEvent *e; // Ptr for the event

    PacketPtr request; // Storing  a packet ptr for our current request

public:
    DRAM(std::shared_ptr<System> sys, AddrRange _addrs, Tick respLatency);

    // Helper functions
    void setDataAddr(Addr, uint8_t*, size_t); // Set the value in dram

    template<typename T>
    void writeAtAddr(Addr, T);

    void writeByteAtAddr(Addr, uint8_t);
    void writeHalfwordAtAddr(Addr, uint16_t);
    void writeWordAtAddr(Addr, uint32_t);
    void writeDoubleWordAtAddr(Addr, uint64_t);

    void getDataAtAddr(Addr, uint8_t*, size_t); // Get a value from dram

    template<typename T>
    T readAtAddr(Addr);
    uint8_t readByteAtAddr(Addr);
    uint16_t readHalfwordAtAddr(Addr);
    uint32_t readWordAtAddr(Addr);
    uint64_t readDoubleWordAtAddr(Addr);

    void process();
    void initialize() override {}
    void recvReq(PacketPtr pkt);

    SlavePort *getPort() { return port; }
    const AddrRange getAddrRange() { return addrs; }
};

#endif //DRAM_H
