#ifndef CPU_HH
#define CPU_HH

#include "register.hh"
#include "simobject.hh"
#include "ports.hh"
#include <memory>
#include <bitset>

// Holds the data between the pipeline stages
template<typename T>
struct Instruction{
    Register rs1, rs2, rs3, rd;
    T immDestination;
    T result;
    T funct3;
    T opcode, funct7;
    T funct2, funct5, imm;

    T data;
    T currentInstruction; // The 32 bit instruction
};

// Base pipeline stage class
class Pipeline{
public:
    Instruction<std::bitset<32>> *inst;
    Pipeline() : inst(new Instruction<std::bitset<32>>()) {}
};

class CPU : public SimObject{
private:
    // Pipeline stages
    // Fetch Stage
    class Fetch : public Pipeline{
    protected:
        // Fetch event for the fetch stage. It calls the processInst to access memory
        class FetchEvent : public Event{
        private:
            Fetch *fetch;
        public:
            FetchEvent(Fetch *_f) : Event(), fetch(_f) {}
            virtual void process() override { fetch->fetchInstruction(); }
                virtual const char* description() override {return "Fetch";}
                void fetchEvent(){
                    std::cout << "Scheduling Fetch Event on Tick " << fetch->cpu->currTick() << std::endl;
                    // cpu->sysMain->removeEvent(); // removing event that was just executed
                    fetch->cpu->schedule(fetch->e, fetch->cpu->currTick() + fetch->cpu->clkTick); // Scheduling new event
                }
        };
        CPU *cpu;
        FetchEvent *e; // Access the FetchEvent class from Fetch or outside Fetch
        bool busy; // Pipleine busy or not

        friend class CPU; // Allows CPU class to access these private variables
    public:
        Fetch(CPU *c) : cpu(c), e(new FetchEvent(this)), busy(0) {}
        void fetchInstruction();
        void setBusy(bool _busy) { busy = _busy; }
        bool isBusy() { return busy; } // Used to determine if the fetch stage is busy
    };

    // Decode Stage
    class Decode : public Pipeline{
    // Finds the data from the registers and passes it to the execution stage to be executed
    private:
        // Decode event for the decode stage
        class DecodeEvent : public Event{
        private:
            Decode *dec;
        public:
            DecodeEvent(Decode *_d) : Event(), dec(_d) {}
            virtual void process() override {
                dec->decodeInstruction();

                // if(cpu->stall->getIsStalled() == false){
                    // cpu->reg->setRegiserAccess(0);
                dec->cpu->ex->e->exEvent();
                // }

            }
            virtual const char* description() override {return "Decode";}
            void decodeEvent(){
                std::cout << "Scheduling Decode Event on Tick " << dec->cpu->currTick() << std::endl;
                dec->cpu->schedule(dec->e, dec->cpu->currTick() + dec->cpu->clkTick); // Scheduling new event
            }
        };
        CPU *cpu;
        DecodeEvent *e; // Access the DecodeEvent class from Decode or outside Decode
        bool busy;

        friend class CPU; // Allows CPU class to access these private variables

    public:
        Decode(CPU *c) : cpu(c), e(new DecodeEvent(this)), busy(0) {}
        void decodeInstruction();
        void setBusy(bool _busy) { busy = _busy; }
        bool isBusy() { return busy; }  // Used to determine if the fetch stage is busy
    };

    // Execute Stage
    class Execute : Pipeline{
    // Passes the incoming registers or memory location to the ALU to be operated
    private:
        // Execute event for the execute stage
        class ExecuteEvent : public Event{
        private:
            Execute *ex;
        public:
            ExecuteEvent(Execute *_ex) : Event(), ex(_ex) {}
            virtual void process() override {
                ex->executeInstruction();
                ex->cpu->s->e->storeEvent();
            }
            virtual const char* description() override {return "Execute";}
            void exEvent(){
                std::cout << "Scheduling Execute Event on Tick " << ex->cpu->currTick() << std::endl;
                ex->cpu->schedule(ex->e, ex->cpu->currTick() + ex->cpu->clkTick); // Scheduling new event
            }
        };
        CPU *cpu;
        ExecuteEvent *e; // Access the ExecuteEvent class from Execute or outside Execute
        bool busy; // Busy or not

        friend class CPU; // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : cpu(c), e(new ExecuteEvent(this)), busy(0) {}
        void executeInstruction();
        void setBusy(bool _busy) { busy = _busy; }
        bool isBusy() { return busy; } // Used to determine if the fetch stage is busy
    };

// Store Stage
class Store : public Pipeline{
private:
    // Store event for the store stage
    class StoreEvent : public Event{
    private:
        Store *s;
    public:
        StoreEvent(Store *_s) : Event(), s(_s) {}
        virtual void process() override {
            s->storeInstruction();
        }
        virtual const char* description() override {return "Store";}
        void storeEvent(){
            std::cout << "Scheduling Store on Tick " << s->cpu->currTick() << std::endl;
            s->cpu->schedule(s->e, s->cpu->currTick() + s->cpu->clkTick); // Scheduling new event
        }
    };
    CPU *cpu;
    StoreEvent *e; // Access the StoreEvent class from Store or outside Store
    bool busy; // Busy or not

    friend class CPU; // Allows CPU class to access these private variables

public:
    Store(CPU *c) : cpu(c), e(new StoreEvent(this)), busy(0) {}
    void storeInstruction();
    void setBusy(bool _busy) { busy = _busy; }
    bool isBusy() { return busy; }
    };

    // Stalls the pipeline
    class Stall : public Event{
    private:
        CPU *cpu;
        size_t stallAmount;
        bool isStalled = false;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Stall(CPU *c) : Event(1), cpu(c) {}
        virtual void process() override {
            // Stall the processor
            cpu->stall->stallCPU();
            cpu->stall->setIsStalled(0);
        }
        virtual const char* description() override { return "Stall"; }
        void stallEvent(){
            std::cout << "Scheduling stall on Tick " << cpu->currTick() << std::endl;
            std::cout << "Stalling for " << stallAmount << " ticks\n" << std::endl;
            cpu->schedule(cpu->stall, cpu->currTick() + 1); // Scheduling new event
        }
        void stallCPU();
        void setAmount(size_t amount) { stallAmount = amount; }
        bool getIsStalled(){ return isStalled; }
        void setIsStalled(bool isStalled){ this->isStalled = isStalled; }
    };

    // ALU for executing the instructions
    class ALU : public Event{
    private:
        CPU *cpu;
        std::deque<size_t> jump;
        // friend class CPU; // Allows CPU class to access these private variables

    public:
        ALU(CPU *c) : Event(1), cpu(c) {}
        virtual void process() override {
            cpu->a->aluOperations();
            // cpu->sysMain->removeEvent();
        }
        void aluEvent(){
            std::cout << "scheduling Alu on Tick " << cpu->currTick() << std::endl;
            // cpu->sysMain->removeEvent(); // removing event that was just executed
            cpu->schedule(cpu->a, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }

        virtual const char* description() override { return "ALU"; }
        void setJump(size_t addrs) { jump.push_back(addrs); }
        std::deque<size_t> getJump() { return jump; }
        size_t getFirstJump() { return jump.front(); }
        void aluOperations();
        void ADDI();
        void SLLI();
        void SW();
        void FSW();
        void FADDS();
        void J();
        void LW();
        void BLT();
        void LUI();
        void FLW();
        void ADD();
        void RET();
        void JALR();
    };

    // For creating a send data event to pass data through the pipeline
    class Send : public Event{
    private:
        CPU *cpu;
    public:
        Send(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
            cpu->send->sendData();
            cpu->sysMain->removeEvent();
        }
        virtual const char* description() override { return "Send Data"; }
        void sendEvent(){
            std::cout << "scheduling Send Data on Tick " << cpu->currTick() << std::endl;
            cpu->schedule(cpu->send, cpu->currTick() + 5); // Scheduling new event
        }
        void sendData();
    };

    // RequestEvent for the data port of the memory
    class RequestDataEvent : public Event{
    private:
        CPU *owner;
    public:
        RequestDataEvent(CPU *_owner) : Event(), owner(_owner) {}
        void process() override { owner->processData(); }
        const char* description() override { return "CPU Data Clock Event"; }
    };

    // RequestPort for the data port of the memory
    class RequestDataPort : public MasterPort{
    private:
        CPU *owner;
        PacketPtr activeRequest;
    public:
        RequestDataPort(CPU *_owner) : MasterPort(), owner(_owner), activeRequest(0) {}
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

    // RequestEvent for the instruction port of the memory
    class RequestInstEvent : public Event{
    private:
        CPU *owner;
    public:
        RequestInstEvent(CPU *_owner) : Event(), owner(_owner) {}
        void process() override { owner->processInst(); }
        const char* description() override { return "CPU Instruction Clock Event"; }
    };

    // RequestPort for the instruction port of the memory
    class RequestInstPort : public MasterPort{
    private:
        CPU *owner;
        PacketPtr activeRequest;
    public:
        RequestInstPort(CPU *_owner) : MasterPort(), owner(_owner), activeRequest(0) {}
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

    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;
    Stall *stall;
    ALU *a;
    Send *send;
    // Register bank for the cpu
    RegisterBank *reg;
    // Ports to access memory
    RequestInstEvent *e1;   // Used to create a request event for instruction memory
    RequestDataEvent *e2;   // Used to create a request event for data memory
    RequestInstPort *port1; // Used to access the request port for instruction memory
    RequestDataPort *port2; // Used to access the request port for data memory

    std::bitset<32> currentInstruction;

    size_t cycles = 1;
    Tick clkTick; // How far in advance that the event is going to be scheduled
    size_t currAddrI; // Current address for the instruction Memory
    size_t currAddrD; // Current address for the data Memory
    size_t endAddrI; // End address for the Instruction Memory
    size_t endAddrD; // End address for the data Memory

public:
    CPU(std::shared_ptr<System> s1, const char* name, size_t start1, size_t end1, size_t start2, size_t end2);
    ~CPU();
    void processInst() {
        if(!(port1->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddrI << " for 4 bytes on Tick: " << currTick() << std:: endl;
            port1->sendReq(new Packet(true, currAddrI, 4));

            currAddrI+=4;
        }

        // Schedules another fetch Event
        if(currAddrI < endAddrI){
            std::cout << "currAddrI: " << currAddrI << std::endl;
            std::cout << "endAddrI: " << endAddrI << std::endl;
            // d->e->decodeEvent(); // Scheduling decode
            f->e->fetchEvent(); // Scheduling fetch
        }
    }
    void processData() {
        if(!(port2->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddrD << " for 4 bytes on Tick: " << currTick() << std:: endl;
            port2->sendReq(new Packet(true, currAddrD, 4));

            currAddrD+=4;
        }

        // Schedules another memory request. We dont want to do this arbitrarily every clock cycle
        // if(currAddrD < endAddrD){
        //     std::cout << "Attempting to schedule CPU Data Clock Event at time" << currTick() << std::endl;
        //     schedule(e2, currTick() + clkTick);
        // }
    }

    void recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;
        // std::cout << getName() << " read: " << *(float *)(pkt->getBuffer()) << std::endl;
        // Reading from memory in decimal
        // std::cout << getName() << " read in decimal: " << *(uint32_t *)(pkt->getBuffer()) << std::endl;

        if(f->isBusy()){
            // Reading from memory in binary
            currentInstruction = *(uint32_t *)(pkt->getBuffer());
            std::cout << getName() << " read in binary: " << currentInstruction << std::endl;
            f->setBusy(0);
        }
    }
    MasterPort *getPort1() { return port1; } // Returns the
    MasterPort *getPort2() { return port2; }

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        std::cout << "Initializing first fetch for: " << getName() << std::endl << std::endl;
        schedule(f->e, currTick() + 1);
    }
    ALU *getALU() { return a; }
};

#endif
