#ifndef MEMREQUESTER_H
#define MEMREQUESTER_H

#include "simobject.hh"
#include "ports.hh"
#include <iostream>

class memrequester : public SimObject{
private:
    class RequestEvent : public Event{
        memrequester *owner;

        RequestEvent(memrequester *_owner) : Event(), owner(_owner) {}
        void process() override { owner->process(); }
        const char* description() override { return "MemRequester Clock Event"; }
    };

    class RequestPort : public MasterPort{
    private:
        memrequester *owner;
        PacketPtr activeRequest;
    public:
        RequestPort(MemRequester *_owner) : MasterPort(), owner(_owner) {}
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

    RequestEvent *e;
    RequestPort *port;
    Tick clk_tick;
    size_t currAddr;
    size_t endAddr;

public:
    MemRequester(System * sys, const char* name, size_t start, size_t end) :
        SimObject(sys, name),
        e(new RequestEvent(this)),
        port(new RequestPort(this)),
        clk_tick(10),
        currAddr(start),
        endAddr(end) {}
    ~MemRequester();
    virtual void initialize() override _{
        schedule(e, currTick());
    }
    void process() {
        if(!(port->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddr << " for 4 bytes" <<std:: endl;
            port->sendReq(new Packet(true, currAddr, 4));
            currAddr+=4;
        }

        if(currAddr < endAddr){
            schedule(e, currTick() + clk_tick);
        }
    }
    void recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet responce from memory on Tick: " << currTick() << std::endl;
        std::cout << getName() << " read: " << *(float *)(pkt->getBuffer()) << std::endl;
    }
    MasterPort *getPort() { return port; }
};


#endif // MEMREQUESTER_H
