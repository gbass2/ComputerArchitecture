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
        Fetch(CPU *c) : cpu(c) {}
            virtual void process() override {
                cpu->f->fetchInstruction();

                // // Scheduling decode for next instruction
                // cpu->d->decodeEvent();
                //
                // // Scheduling fetch for next instruction
                cpu->f->fetchEvent();
                //
                // // Creating a send data event
                // cpu->send->sendEvent();
                //
                // cpu->f->setIsMemAccess(true);
            }
            virtual const char* description() override {return "Fetch";}
            void fetchEvent(){
                std::cout << "Scheduling Fetch on Tick " << cpu->currTick() << std::endl;
                cpu->sysMain->removeEvent(); // removing event that was just executed

                if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
                    cpu->schedule(cpu->f, cpu->currTick() + cpu->clkTick); // Scheduling new event
                else
                    cpu->schedule(cpu->f, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
                cpu->cycles++;
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

            if(cpu->stall->getIsStalled() == false){
                cpu->sysMain->removeEvent();
                cpu->reg->setRegiserAccess(0);
            //     cpu->ex->exEvent();
            }

        }
        virtual const char* description() override {return "Decode";}
        void decodeEvent(){
            std::cout << "Scheduling Decode on Tick " << cpu->currTick() << std::endl;

            if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
                cpu->schedule(cpu->d, cpu->currTick() + cpu->clkTick); // Scheduling new event
            else
                cpu->schedule(cpu->d, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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
            //     // // cpu->s->storeEvent();
                cpu->sysMain->removeEvent();

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
            cpu->sysMain->removeEvent(); // removing event that was just executed

            if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
                cpu->schedule(cpu->ex, cpu->currTick() + cpu->clkTick); // Scheduling new event
            else
                cpu->schedule(cpu->ex, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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

        }
        virtual const char* description() override {return "Store";}
        void storeEvent(){
            std::cout << "Scheduling Store on Tick " << cpu->currTick() << std::endl;
            cpu->sysMain->removeEvent(); // removing event that was just executed

            if((cpu->currTick() + 10 == 11 + (cpu->cycles)*10))
                cpu->schedule(cpu->s, cpu->currTick() + cpu->clkTick); // Scheduling new event
            else
                cpu->schedule(cpu->s, ((cpu->cycles + 1)*(cpu->clkTick+1))); // Scheduling new event
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
            cpu->sysMain->removeEvent();
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
            cpu->sysMain->removeEvent();
        }
        void aluEvent(){
            std::cout << "scheduling Alu on Tick " << cpu->currTick() << std::endl;
            cpu->sysMain->removeEvent(); // removing event that was just executed
            cpu->schedule(cpu->a, cpu->currTick() + cpu->clkTick); // Scheduling new event
        }

        virtual const char* description() override { return "ALU"; }
        void setJump(size_t addrs) { jump.push_back(addrs); }
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

    class RequestEvent : public Event{
    private:
        CPU *owner;
    public:
        RequestEvent(CPU *_owner) : Event(), owner(_owner) {}
        void process() override { owner->process(); }
        const char* description() override { return "CPU Clock Event"; }
    };

    class RequestPort : public MasterPort{
    private:
        CPU *owner;
        PacketPtr activeRequest;
    public:
        RequestPort(CPU *_owner) : MasterPort(), owner(_owner) {}
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
    RequestEvent *e;
    RequestPort *port;

    size_t cycles = 1;
    size_t PC = 0; // Program Counter
    Tick clkTick; // How far in advance that the event is going to be scheduled
    size_t currAddr;
    size_t endAddr;

public:
    CPU(std::shared_ptr<System> s1, const char* name, size_t start, size_t end) :
        SimObject(s1, name),
        f(new Fetch(this)),
        d(new Decode(this)),
        ex(new Execute(this)),
        s(new Store(this)),
        stall(new Stall(this)),
        a(new ALU(this)),
        send(new Send(this)),
        reg(new RegisterBank(s1)),
        e(new RequestEvent(this)),
        port(new RequestPort(this)),
        clkTick(10),
        currAddr(start),
        endAddr(end) {}
    ~CPU();
    void process() {
        if(!(port->isBusy())){
            std::cout << "Creating memory request to Addr: " << currAddr << " for 4 bytes on Tick: " << currTick() << std:: endl;
            port->sendReq(new Packet(true, currAddr, 4));

            currAddr+=4;
        }

        // Schedules another memory request. We dont want to do this arbitrarily every clock cycle
        // if(currAddr < endAddr){
        //     schedule(e, currTick() + clkTick);
        // }
    }

    void recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet responce from memory on Tick: " << currTick() << std::endl;
        std::cout << getName() << " read: " << *(float *)(pkt->getBuffer()) << std::endl;

        // The read from memory ends up here so, have a variable to store the data until it is stored from whoever called it.
    }
    MasterPort *getPort() { return port; }

    virtual void initialize() override { // Initialzes MEQ with a fetch event
        setupSimulator();

        schedule(f, currTick()); // Schedules the first fetch event
    }

    void setupSimulator(); // Sets up the instruction memory with the instructions
};
