#ifndef CPU_HH
#define CPU_HH

#include "register.hh"
#include "simobject.hh"
#include "ports.hh"
#include <memory>
#include <bitset>

class CPU : public SimObject{
private:
    // Pipeline stages
    // Fetch Stage
    class Fetch : public Event{
    protected:
        CPU *cpu;
        bool isMemAccess = true;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Fetch(CPU *c) : Event(), cpu(c) {}
            virtual void process() override {
                cpu->f->fetchInstruction();

                // // Scheduling decode for next instruction
                if(cpu->currAddrI < cpu->endAddrI){
                    cpu->d->decodeEvent();
                    //
                    // // Scheduling fetch for next instruction
                    cpu->f->fetchEvent();
                    //
                    // // Creating a send data event
                    // cpu->send->sendEvent();
                    //
                    // cpu->f->setIsMemAccess(true);
                }

                cpu->currAddrI += 4;
            }
            virtual const char* description() override {return "Fetch";}
            void fetchEvent(){
                std::cout << "Scheduling Fetch on Tick " << cpu->currTick() << std::endl;
                // cpu->sysMain->removeEvent(); // removing event that was just executed
                cpu->schedule(cpu->f, cpu->currTick() + cpu->clkTick); // Scheduling new event
            }
        void fetchInstruction(); // Gets the instruction from the instruction memory
        void setIsMemAccess(bool isMemAccess){ this->isMemAccess = isMemAccess; }
    };

    // Decode Stage
    class Decode : public Event{
    // Finds the data from the registers and passes it to the execution stage to be executed
    private:
        CPU *cpu;
        std::bitset<32> rs1, rs2, rs3;
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        std::bitset<32> result;
        std::bitset<3> funct3;
        std::bitset<7> opcode, funct7;
        std::string funct2, funct5, imm;
        friend class CPU; // Allows Send class to access these private variables

    public:
        Decode(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
            cpu->d->decodeInstruction();

            // if(cpu->stall->getIsStalled() == false){
                // cpu->sysMain->removeEvent();
                // cpu->reg->setRegiserAccess(0);
                cpu->ex->exEvent();
            // }

        }
        virtual const char* description() override {return "Decode";}
        void decodeEvent(){
            std::cout << "Scheduling Decode on Tick " << cpu->currTick() << std::endl;
            cpu->schedule(cpu->d, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }
        void decodeInstruction();
    };

    // Execute Stage
    class Execute : public Event{
    // Passes the incoming registers or memory location to the ALU to be operated
    private:
        CPU *cpu;
        std::bitset<32> rs1, rs2, rs3;
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        std::bitset<32> result;
        std::bitset<3> funct3;
        std::bitset<7> opcode, funct7;
        std::string funct2, funct5, imm;
        bool isMemAccess = false;
        // Instruction currentInstruction;
        friend class CPU; // Allows CPU class to access these private variables

    public:
        Execute(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
            cpu->ex->executeInstruction();
            // // if(cpu->stall->getIsStalled() == false){
                cpu->s->storeEvent();
                // cpu->sysMain->removeEvent();

            // Check the instruction set to determine latency time
            //     if(cpu->ex->getInstruction().getInstSet() == "Base"){
            //         // Creating the Instruction latency by stalling
            //         stall->setIsStalled(1);
            //         stall->setAmount(8);
            //         stall->process();
            //     }
            // // }
        }
        virtual const char* description() override {return "Execute";}
        void exEvent(){
            std::cout << "Scheduling Execute on Tick " << cpu->currTick() << std::endl;
            // cpu->sysMain->removeEvent(); // removing event that was just executed
             cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
            // if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            //     cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
            // else
            //     cpu->schedule(cpu->ex, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
        }
        void executeInstruction();
        void setIsMemAccess(bool isMemAccess){ this->isMemAccess = isMemAccess; }
        // void setInstruction(Instruction currentInstruction) { this->currentInstruction = currentInstruction; }
        // Instruction getInstruction() { return currentInstruction;}

    };

    // Store Stage
    class Store : public Event{
    // Store back in memory if needed
    private:
        CPU *cpu;
        std::bitset<5> rd;
        std::bitset<32 >immDestination;
        friend class CPU; // Allows CPU class to access these private variables
    public:
        Store(CPU *c) : Event(0), cpu(c) {}
        virtual void process() override {
            cpu->s->storeInstruction();
        }
        virtual const char* description() override {return "Store";}
        void storeEvent(){
            std::cout << "Scheduling Store on Tick " << cpu->currTick() << std::endl;
            // cpu->sysMain->removeEvent(); // removing event that was just executed
            cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
            // if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
            //     cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
            // else
            //     cpu->schedule(cpu->s, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
        }
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
        Stall(CPU *c) : Event(1), cpu(c) {}
        virtual void process() override {
            // Stall the processor
            // cpu->sysMain->removeEvent();
            cpu->stall->stallCPU();
            cpu->stall->setIsStalled(0);
            // Set isStalled to 0 where ever the stall event was created the stall event
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
    RegisterBank *reg;
    RequestInstEvent *e1;   // Used to create a request event for instruction memory
    RequestDataEvent *e2;   // Used to create a request event for data memory
    RequestInstPort *port1; // Used to access the request port for instruction memory
    RequestDataPort *port2; // Used to access the request port for data memory

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

        // Schedules another memory request. We dont want to do this arbitrarily every clock cycle
        // if(currAddrI < endAddrI){
        //     std::cout << "Attempting to schedule CPU Data Clock Event at time" << currTick() << std::endl;
        //     schedule(e1, currTick() + clkTick);
        // }
    }
    void processData() {
        if(!(port2->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddrD << " for 4 bytes on Tick: " << currTick() << std:: endl;
            port2->sendReq(new Packet(true, currAddrD, 4));

            currAddrD+=4;
        }

        // Schedules another memory request. We dont want to do this arbitrarily every clock cycle
        if(currAddrD < endAddrD){
            std::cout << "Attempting to schedule CPU Data Clock Event at time" << currTick() << std::endl;
            schedule(e2, currTick() + clkTick);
        }
    }

    void recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;
        // std::cout << getName() << " read: " << *(float *)(pkt->getBuffer()) << std::endl;
        std::cout << getName() << " read: " << *(uint32_t *)(pkt->getBuffer()) << std::endl;

        // The read from memory ends up here so, have a variable to store the data until it is stored from whoever called it.
    }
    MasterPort *getPort1() { return port1; } // Returns the
    MasterPort *getPort2() { return port2; }

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        schedule(e1, currTick()); // Schedules the first fetch event
        // std::cout << "Initializing first fetch for: " << getName() << std::endl << std::endl;
        // schedule(f, currTick() + 1); // Schedules the first fetch event
    }
    ALU *getALU() { return a; }
};

#endif
