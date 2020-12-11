#ifndef CAHCE_HH
#define CACHE_HH

#include "ports.hh"
#include "simboject.hh"

#include <memory>
#include <vector>

class Cache : public SimObject{
private:

    // ********************************************************************
    // Port that points upwards toward the CPU in the memory hiearchy
    // ********************************************************************
    class CPUSidePort : public SlavePort{
    private:
        Cache *owner;
    public:
        CPUSidePort(Membus *_owner): SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt); }
        const AddrRange getAddrRange() override { return AddrRange(0,0); }
    };

    // RequestPort for the cache to membus
    class RequestPort : public MasterPort{
    private:
        Cahce *owner;
        PacketPtr activeRequest;
    public:
        RequestPort(Cache *_owner) : MasterPort(), owner(_owner), activeRequest(0) {}
        bool isBusy() { return (activeRequest); }
        void recvResp(PacketPtr pkt) override {
            activeRequest = nullptr;
            owner->recvResp(pkt);
        }
        void sendReq(PacketPtr pkt) override{
            activeRequest = pkt;
            MasterPort::sendReq(pkt);
        }
    };

    RequestPort *port; // Used to access the request port for instruction memory
    size_t byteAmount;
    size_t currAddrs;
    size_t endAddrs;
    std::string name;
    std::vector<CPUSidePort *> cpuSidePorts; // Cpu side connections

public:
    Cahce(std::shared_ptr<System> sys, std::string _name);
    void process();
    void initialize() override {}
    void recvReq();
    void recvResp(PacketPtr pkt);
    MasterPort *getPort1() { return port1; } // Returns the instruction port
    MasterPort *getPort2() { return port2; } // Returns the data port
};

#endif

/*
Cpu side recieves a req and then stores/access cache.
On a miss it calls process to get a memory access
*/
