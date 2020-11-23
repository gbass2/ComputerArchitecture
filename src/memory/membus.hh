#ifndef MEMBUS_HH
#define MEMBUS_HH

#include "simobject.hh"
#include "ports.hh"

#include <vector>
#include <deque>

class Membus : public SimObject{
private:
    // ********************************************************************
    // Packets take time to filter through the membus. This event manages
    // this process
    // ********************************************************************
    class MembusForwardEvent : public Event{
    private:
        Membus *owner;
    public:
        MembusForwardEvent(Membus *_owner) : Event(), owner(_owner) {}
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
        MemSidePort(Membus * _owner) : owner(_owner) {}
        bool isBusy() { return (activeRequest); }
        void recvResp(PacketPtr pkt) override {
            activeRequest = nullptr;
            owner->recvResp(pkt);
        }
        void sendReq(PacketPtr pkt) override {
            activeRequest = pkt;
            MasterPort::sendReq(pkt);
        }
    };
    // ********************************************************************
    // Port that points upwards toward the CPU in the memory hiearchy
    // ********************************************************************
    class CPUSidePort : public SlavePort{
    private:
        Membus *owner;
    public:
        CPUSidePort(Membus *_owner): SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt); }
        const AddrRange getAddrRange() override { return AddrRange(0,0); }
    };

    typedef std::pair<Tick, PacketPtr> fwdQType;

    Tick fwd_time;
    MembusForwardEvent * fwd_event;
    // Attach multiple ports on both the cpu and memory side
    std::vector<MemSidePort *> memSidePorts; // memory Side connections
    std::vector<CPUSidePort *> cpuSidePorts; // Cpu side connections
    std::deque<PacketPtr> packetsWaitingForMemPorts; // Packets that are waiting for memory port to be able to send to memory
    std::deque<fwdQType> packetsWaitingForForward; // Packet forwarding mechanism
public:
    Membus(std::shared_ptr<System> sys, Tick forward_time);
    void initialize() override {}
    void tryToSend();
    void forwardPackets();
    void recvReq(PacketPtr pkt);
    void recvResp(PacketPtr pkt);
    MemSidePort *getRequestPort(PacketPtr pkt);
    CPUSidePort *getResponsePort(PacketPtr pkt);

    MemSidePort *getMemSidePort(size_t index);
    CPUSidePort *getCPUSidePort(size_t index);
    CPUSidePort *getUnboundCPUSidePort();
    MemSidePort *getUnboundMemSidePort();
};






#endif // MEMBUS_HH
