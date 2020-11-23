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
    // Port that points downward toward Instruction DRAM in the memory hiearchy
    // ********************************************************************
    class MemSidePortI : public MasterPort{
    private:
        Membus *owner;
        PacketPtr activeRequest;
    public:
        MemSidePortI(Membus * _owner) : owner(_owner) {}
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
    // Port that points downward toward Data DRAM in the memory hiearchy
    // ********************************************************************
    class MemSidePortD : public MasterPort{
    private:
        Membus *owner;
        PacketPtr activeRequest;
    public:
        MemSidePortD(Membus * _owner) : owner(_owner) {}
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
    // Port that points upwards toward the Instruction port of CPU in the memory hiearchy
    // ********************************************************************
    class CPUSidePortI : public SlavePort{
    private:
        Membus *owner;
    public:
        CPUSidePortI(Membus *_owner): SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt); }
        const AddrRange getAddrRange() override { return AddrRange(0,0); }
    };

    // ********************************************************************
    // Port that points upwards toward the Data port of CPU in the memory hiearchy
    // ********************************************************************
    class CPUSidePortD : public SlavePort{
    private:
        Membus *owner;
    public:
        CPUSidePortD(Membus *_owner): SlavePort(), owner(_owner) {}
        virtual void recvReq(PacketPtr pkt) override { owner->recvReq(pkt); }
        const AddrRange getAddrRange() override { return AddrRange(0,0); }
    };

    typedef std::pair<Tick, PacketPtr> fwdQType;

    Tick fwd_time;
    MembusForwardEvent * fwd_event;
    // Attach multiple ports on both the cpu and memory side
    std::vector<MemSidePortI *> memSideIPorts; // memory Side connections
    std::vector<MemSidePortI *> memSideDPorts; // memory Side connections
    std::vector<CPUSidePortI *> cpuSideIPorts; // Cpu side connections
    std::vector<CPUSidePortD *> cpuSideDPorts; // Cpu side connections
    std::deque<PacketPtr> packetsWaitingForMemPorts; // Packets that are waiting for memory port to be able to send to memory
    std::deque<fwdQType> packetsWaitingForForward; // Packet forwarding mechanism
public:
    Membus(std::shared_ptr<System> sys, Tick forward_time);
    void initialize() override {}
    void tryToSend();
    void forwardPackets();
    void recvReq(PacketPtr pkt);
    void recvResp(PacketPtr pkt);
    MemSidePortI *getRequestIPort(PacketPtr pkt);
    MemSidePortD *getRequestDPort(PacketPtr pkt);
    CPUSidePortI *getResponsePortI(PacketPtr pkt);
    CPUSidePortD *getResponsePortD(PacketPtr pkt);

    MemSidePortI *getMemSideIPort(size_t index);
    MemSidePortD *getMemSideDPort(size_t index);
    CPUSidePortI *getCPUSidePortI(size_t index);
    CPUSidePortD *getCPUSidePortD(size_t index);
    CPUSidePortI *getUnboundCPUSidePortI();
    CPUSidePortD *getUnboundCPUSidePortD();
    MemSidePortI *getUnboundMemISidePort();
    MemSidePortD *getUnboundMemDSidePort();
};






#endif // MEMBUS_HH
