#ifndef CPU_HH
#define CPU_HH

#include "register.hh"
#include "simobject.hh"
#include "ports.hh"
#include <cmath>
#include <memory>
#include <bitset>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <vector>

// Holds the data between the pipeline stages
template<typename T>
struct Instruction{
    Register<T> rs1, rs2, rs3, rd; // Registers for the current instruction

    std::bitset<3> funct3;
    std::bitset<7> opcode, funct7;
    std::bitset<2> funct2;
    std::bitset<5> funct5;
    std::bitset<12> immISB;
    std::bitset<20> immJU;
    std::bitset<32> currentInstruction; // The 32 bit instruction

    T data;
    std::string type; // Type of instruction

    void flush(){ // flush/clear the pipeline stages if jumped or branch was taken
        currentInstruction.reset();
        funct2.reset();
        funct3.reset();
        funct5.reset();
        funct7.reset();
        opcode.reset();
        immISB.reset();
        immJU.reset();
        rd.setName(0);
    }
};

// Base pipeline stage class
class Pipeline{
protected:
    bool busy; // Pipleine busy or not
    bool read; // Memory read or write
    bool memAccess; // Memory access or not for the execute stage
    bool memAccessFinished;
    bool isFloat;
    bool flushed; // After Jump/branch and if we take it then we have to get rid of the instruction that is in fetch
    size_t latency; // the latency involved based on what instruction set is being processed

public:
    Instruction<int> intInst;
    Instruction<float> fInst;

    Pipeline(): busy(0), read(1), memAccess(0), memAccessFinished(1), isFloat(0), flushed(0), latency(0) {}
    void setBusy(bool _busy) { busy = _busy; }
    void setRead(bool _read) { read = _read; }
    void setMemAccess(bool _memAccess) { memAccess = _memAccess; }
    void setFloat(bool _isFloat) { isFloat = _isFloat; }
    void setLatency(size_t _latency) { latency = _latency; }
    void setMemAccessFinished(bool _memAccessFinished) { memAccessFinished = _memAccessFinished; }
    void setFlushed(bool _flushed) {flushed = _flushed; }
    bool isBusy() { return busy; }
    bool isRead() { return read; }
    bool isMemAccess() { return memAccess; }
    bool getIsFloat() { return isFloat; }
    bool isMemAccessFinished() { return memAccessFinished; }
    size_t getLatency() { return latency; }
    bool isFlushed() { return flushed; }
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
                    // size_t n = fetch->cpu->currTick();
                    size_t eventTime = fetch->cpu->currTick() + 10;
                    fetch->cpu->schedule(fetch->e, eventTime); // Scheduling new event
                }
        };
        class ReleaseEvent : public Event{
        private:
            Fetch *fetch;
        public:
            ReleaseEvent(Fetch *_f) : Event(), fetch(_f) {}
            virtual void process() override {
                if(fetch->cpu->ex->isBusy())                                // If execute is busy then Reschedule
                    fetch->cpu->schedule(this, fetch->cpu->currTick() + 10);
                else {
                    std::cout << "Fetch has released it's data" << std::endl;                                            // If not then send data to decode and schedule and schedule decode and fetch
                    // Passing the instruction of fetch to decode for int instruction
                    fetch->cpu->d->intInst = fetch->intInst;
                    // Passing the instruction of fetch to decode for float instruction
                    fetch->cpu->d->fInst = fetch->fInst;

                    fetch->cpu->d->setRead(fetch->isRead());
                    fetch->cpu->d->setMemAccess(fetch->isMemAccess());
                    fetch->cpu->d->setFloat(fetch->getIsFloat());

                    if(fetch->cpu->currAddrI >= fetch->cpu->endAddrI)
                        fetch->cpu->d->e->decodeEvent();
                    // create a fetch event after data is released
                    if(fetch->cpu->currAddrI < fetch->cpu->endAddrI){
                        fetch->cpu->d->e->decodeEvent();
                        fetch->e->fetchEvent(); // Scheduling fetch
                    }

                    fetch->setBusy(0);
                }
            }
            virtual const char* description() override {return "Fetch Release Event";}
            void releaseEvent() {
                std::cout << "Scheduling Fetch Release Event on Tick: " << fetch->cpu->currTick() << std::endl;
                // size_t n = fetch->cpu->currTick();
                size_t eventTime = fetch->cpu->currTick() + 5;
                fetch->cpu->schedule(this, eventTime); // Scheduling new event
            }

        };
        CPU *cpu;
        FetchEvent *e; // Access the FetchEvent class from Fetch or outside Fetch
        ReleaseEvent *release;  // Release the data

        friend class CPU; // Allows CPU class to access these private variables
    public:
        Fetch(CPU *c) : cpu(c), e(new FetchEvent(this)), release(new ReleaseEvent(this)) {}
        void fetchInstruction();
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
            }
            virtual const char* description() override {return "Decode";}
            void decodeEvent(){
                std::cout << "Scheduling Decode Event on Tick: " << dec->cpu->currTick() << std::endl;
                // size_t n = dec->cpu->currTick();
                size_t eventTime = dec->cpu->currTick() + 10;
                dec->cpu->schedule(dec->e, eventTime); // Scheduling new event
            }
        };
        class ReleaseEvent : public Event{
        private:
            Decode *d;
        public:
            ReleaseEvent(Decode *_d) : Event(), d(_d) {}
            virtual void process() override {
                if(d->cpu->ex->isBusy())                                // If execute is busy then Reschedule
                    d->cpu->schedule(this, d->cpu->currTick() + 10);
                else {
                    std::cout << "Decode has released it's data" << std::endl;                                            // If not then send data to execute and schedule an execute
                    d->cpu->ex->intInst = d->intInst;                   // Passing the instruction of decode to execute for int instruction

                    d->cpu->ex->fInst = d->fInst;                       // Passing the instruction of decode to execute for float instruction
                    // std::cout << "rs1 name: " << d->cpu->ex->intInst.rs1.getName() << std::endl;
                    d->cpu->ex->setRead(d->isRead());
                    d->cpu->ex->setMemAccess(d->isMemAccess());
                    d->cpu->ex->setFloat(d->getIsFloat());
                    d->cpu->ex->setLatency(d->getLatency());


                    d->cpu->ex->e->exEvent(); // create an execute event after data is released

                    d->setBusy(0);
                }
            }
            virtual const char* description() override {return "Decode Release Event";}
            void releaseEvent() {
                std::cout << "Scheduling Decode Release Event on Tick: " << d->cpu->currTick() << std::endl;
                // size_t n = d->cpu->currTick();
                size_t eventTime = d->cpu->currTick() + 5;
                d->cpu->schedule(this, eventTime); // Scheduling new event
            }

        };
        CPU *cpu;
        DecodeEvent *e;     // Access the DecodeEvent class from Decode or outside Decode
        ReleaseEvent *release;  // Release the data
        friend class CPU;   // Allows CPU class to access these private variables

    public:
        Decode(CPU *c) : cpu(c), e(new DecodeEvent(this)), release(new ReleaseEvent(this)) {}
        void decodeInstruction();
        void findInstructionType();
    };

    // Execute Stage
    class Execute : public Pipeline{
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

            }
            virtual const char* description() override {return "Execute";}
            void exEvent(){
                std::cout << "Scheduling Execute Event on Tick: " << ex->cpu->currTick() << std::endl;
                // size_t n = ex->cpu->currTick();
                size_t eventTime = ex->cpu->currTick() + 10;
                ex->cpu->schedule(ex->e, eventTime); // Scheduling new event
            }
        };
        class ReleaseEvent : public Event{
        private:
            Execute *ex;
        public:
            ReleaseEvent(Execute *_ex) : Event(), ex(_ex) {}
            virtual void process() override {
                if(!ex->isMemAccess()) // If we dont need to access memory then we can go ahead and send data
                    ex->setMemAccessFinished(1);


                // If not busy pass data to store and create a store event
                if(ex->isMemAccessFinished()){
                    std::cout << "Execute has released it's data" << std::endl;
                    // Passing the instruction of execute to store for int instruction
                    ex->cpu->s->intInst = ex->intInst;
                    // Passing the instruction of execute to store for float instruction
                    ex->cpu->s->fInst = ex->fInst;

                    ex->cpu->s->setRead(ex->cpu->ex->isRead());
                    ex->cpu->s->setMemAccess(ex->cpu->ex->isMemAccess());
                    ex->cpu->s->setFloat(ex->cpu->ex->getIsFloat());

                    // create a store after data is released
                    if((ex->isMemAccess() && ex->isRead()) || !ex->isMemAccess())
                        ex->cpu->s->e->storeEvent();

                    ex->setBusy(0);
                } else
                    ex->cpu->schedule(this, ex->cpu->currTick() + 10); // Reschedule if busy for current tick + 10
            }
            virtual const char* description() override {return "Execute Release Event";}
            void releaseEvent() {
                std::cout << "Scheduling Execute Release Event on Tick: " << ex->cpu->currTick() << std::endl;
                // size_t n = ex->cpu->currTick();
                size_t eventTime = ex->cpu->currTick()  +  ex->getLatency(); // Schedule a release event after the memory has finished it's events and after the alu latency
                ex->cpu->schedule(this, eventTime); // Scheduling new event
            }

        };
        CPU *cpu;
        ExecuteEvent *e;        // Access the ExecuteEvent class from Execute or outside Execute
        ReleaseEvent *release;  // Release the data from execute after a latency from the alu operation
        friend class CPU;       // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : cpu(c), e(new ExecuteEvent(this)), release(new ReleaseEvent(this)) {}
        void executeInstruction();
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
            std::cout << "Scheduling Store on Tick: " << s->cpu->currTick() << std::endl;
            size_t n = s->cpu->currTick();
            size_t eventTime = (n >= 0 ? ((n + 10 - 1) / 10) * 10 : (n / 10) * 10) + 1;
            s->cpu->schedule(s->e, eventTime); // Scheduling new event
        }
    };
    CPU *cpu;
    StoreEvent *e;    // Access the StoreEvent class from Store or outside Store
    friend class CPU; // Allows CPU class to access these private variables

public:
    Store(CPU *c) : cpu(c), e(new StoreEvent(this)) {}
    void storeInstruction();
    };

    // Stalls the pipeline
    class Stall : public Event{
    private:
        CPU *cpu;
        size_t stallAmount;
        bool isStalled = false;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Stall(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
            // Stall the processor
            cpu->stall->stallCPU();
            cpu->stall->setIsStalled(0);
        }
        virtual const char* description() override { return "Stall"; }
        void stallEvent(){
            std::cout << "Scheduling stall on Tick: " << cpu->currTick() << std::endl;
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
        ALU(CPU *c) : Event(), cpu(c) {}
        virtual void process() override {
            cpu->a->aluOperations();
        }
        void aluEvent(){
            std::cout << "Scheduling Alu on Tick: " << cpu->currTick() << std::endl;
            cpu->schedule(cpu->a, cpu->currTick() + 1); // Scheduling new event
        }

        virtual const char* description() override { return "ALU"; }
        void setJump(size_t addrs) { jump.push_back(addrs); }
        std::deque<size_t> getJump() { return jump; }
        size_t getFirstJump() { return jump.front(); }
        void aluOperations();
        void ADD();
        void ADDI();
        void SLLI(); void SRLI(); void SRAI(); void SLTI(); void SLTIU();
        void XORI(); void ORI(); void ANDI();
        void SB(); void SH(); void SW();
        void JAL();   void JALR();
        void LB();  void LH(); void LW(); void LBU(); void LHU();
        void BEQ(); void BNE(); void BGE(); void BLT(); void BLTU(); void BGEU();
        void LUI(); void AUIPC();
        void FLW();
        void FSW();
        void FADDS();
        void FSUBS();
        void MUL();
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

public:
    // Instances of the pipeline stages
    Fetch *f;
    Decode *d;
    Execute *ex;
    Store *s;
    Stall *stall;
    ALU *a;

    RegisterBank *reg; // Variable to access RegisterBank

    // Ports to access memory
    RequestInstEvent *e1;   // Used to create a request event for instruction memory
    RequestDataEvent *e2;   // Used to create a request event for data memory
    RequestInstPort *port1; // Used to access the request port for instruction memory
    RequestDataPort *port2; // Used to access the request port for data memory

    size_t cycles = 1;
    size_t numInstructions = 0;
    Tick clkTick;       // How far in advance that the event is going to be scheduled
    size_t currAddrI;   // Current address for the instruction Memory
    size_t currAddrD;   // Current address for the data Memory
    size_t endAddrI;    // End address for the Instruction Memory
    size_t endAddrD;    // End address for the data Memory
    size_t byteAmount;  // Amount of bytes need from memory
    friend RegisterBank;

public:
    std::vector<float> output;
    CPU(std::shared_ptr<System> s1, const char* name, size_t start1, size_t end1, size_t start2, size_t end2);
    ~CPU();
    void processInst() {
        if(!(port1->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddrI << " for 4 bytes on Tick: " << currTick() << std:: endl;
            port1->sendReq(new Packet(true, currAddrI, 4, "fetch")); // 4 bytes

            currAddrI+=4;
        }
    }
    void processData() {
        if(!(port2->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddrD << " for " << byteAmount << " bytes on Tick: " << currTick() << std:: endl;
            if(ex->isRead()){
                port2->sendReq(new Packet(true, currAddrD, byteAmount, "execute"));
            }
            else{ // Stroing to memory
                if(!ex->getIsFloat()){
                    std::cout << "data in processData: " << ex->intInst.data << std::endl;
                    if(byteAmount == 1){ // Storing a byte
                        int8_t val = ex->intInst.data;
                        port2->sendReq(new Packet(false, currAddrD, (uint8_t *)(&val), byteAmount, "execute"));
                    } else if(byteAmount == 2){ // Storing half word
                        std::cout << "Half Word" << std::endl;
                        int16_t val = ex->intInst.data;
                        std::cout << "Data: " << ex->intInst.data << std::endl;
                        std::cout << "Stored Value: " << val << std::endl;
                        port2->sendReq(new Packet(false, currAddrD, (uint8_t *)(&val), byteAmount, "execute"));
                    } else if(byteAmount == 4){ // Storing word
                        int32_t val = ex->intInst.data;
                        port2->sendReq(new Packet(false, currAddrD, (uint8_t *)(&val), byteAmount, "execute"));
                    }
                } else { // Store floating point
                    port2->sendReq(new Packet(false, currAddrD, (uint8_t *)(&ex->fInst.data), byteAmount, "execute"));
                }
            }
        }
    }

    void recvResp(PacketPtr pkt);
    MasterPort *getPort1() { return port1; } // Returns the instruction port
    MasterPort *getPort2() { return port2; } // Returns the data port

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        std::cout << "Initializing first fetch for " << getName() << std::endl << std::endl;
        schedule(f->e, currTick() + 1);
    }
    ALU *getALU() { return a; }
    void findInstType();
    void setRegister(int stackBegin, int stackEnd){
        reg->intRegisters[1000].setData(stackBegin);  // Setting the frame ptr
        reg->intRegisters[10].setData(stackEnd);      // Setting stack ptr
    }
};

#endif
