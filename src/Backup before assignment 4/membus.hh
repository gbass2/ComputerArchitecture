#ifndef MEMBUS_HH
#define MEMBUS_HH

#include "simobject.hh"
#include "ports.hh"

#include <vector>
#include <deque>

class Membus : public SimObject{
private:
    // ********************************************************************
    // Packets take time to filter through the membuc. This event manages
    // this process
    // ********************************************************************
    class MembusForwardEvent : public Event{
    private:
        Membus *owner;
    public:
        MembusForwardEvent(Membus *_owner) : event(), owner(_owner) {}
        void process() override { owner->forwardPackets(); }
        const char* description() override { return "Membus Forward Event"; }

    };
    // ********************************************************************
    // Port that points downward toward DRAM in the memory hiearchy
    // ********************************************************************
    class MemSidePort : public MasterPort{
    private:
    Membus *owner;
        PacketPtr activeRequest;
    public:
        MemSidePort(Membus * _owner) : Event(), owner(_owner) {}
        bool isBusy() { return (activeRequest) }
        void recvReq(Packetptr pkt) override {
            activeRequest = nullptr;
            owner->recvResp(pkt);
        }
        void sendReq(PacketPtr pkt){
            activeRequest = pkt;
            MasterPort::sendReq(pkt);
        }
    };
    // ********************************************************************
    // Port that points downward toward DRAM in the memory hiearchy
    // ********************************************************************
    class CPUSidePort : public SlavePort{
    private:
        membus *owner;
    public:
        CPUSidePort(membus *_owner): SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt) }
        const AddrRange getAddrRange() override { return AddrRange(0,0) }
    };

    typedef std::pair<Tick, PacketPtr> fwdQType;

    Tick fwd_time;
    MembusForwardEvent * fwd_event;
    // Attach multiple ports on both the cpu and memory side
    std::vector<MemSidePort *> MemSidePorts; // memory Side connections
    std::vector<CPUSidePort> cpuSidePorts; // Cpu side connections
    std::deque<PacketPtr> packetsWaitingForMemPorts; // Packets that are waiting for memory port to be able to send to memory
    std::deque<fwdQType> packetsWaitingForForward; // Packet forwarding mechanism
public:
    Membus(System *sys, Tick forward_time);

    void tryToSend();
    void forwardPackets();
    void recvReq(PacketPtr pkt);
    void recvResp(PacketPtr pkt);
    MemSideport *getRequestPort(PacketPtr pkt);
    CPUSidePort *getResponsePort(PacketPtr pkt);

    MemSideport *getMemSidePort(size_t index);
    CPUSidePort *getCPUSidePort(size_t index);
    MemSidePort *getUnboundCPUSidePort();
    CPUSidePort *getUnboundMemSidePort();

};






#endif // MEMBUS_HH
