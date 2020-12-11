#include "cache.hh"

Cache::Cache(std::shared_ptr<System> sys, size_t start ,size_t end std::string _name) :
    SimObject(sys),
    port(new RequestPort(this)),
    byteAmount(0),
    currAddrs(start),
    endAddrs(end),
    name(_name) {}

void Cache::process(){
    if(!(port->isBusy())){
        std::cout << "Creating memory request to Addr: " << currAddr << " for " << byteAmount <<  " bytes on Tick: " << currTick() << std:: endl;
        port1->sendReq(new Packet(true, currAddr, 4, name)); // 4 bytes
    }

    currAddr += 4;
}

void Cache::recvResp(PacketPtr pkt){
        std::cout << getName() << " received packet response from memory on Tick: " << currTick() << std::endl;

        // Temp: Execute for load data should NOT be passing through this if statement
        if((pkt->getName() == "fetch" && pkt->isRead())){         // only true for fetch stage
            // Reading from memory in binary
            bitset<32> instruction = *(uint32_t *)(pkt->getBuffer());
            cout << getName() << " read in binary: " << instruction << endl << endl;    // example: cpu0 read in binary: 01010101010

            if(f->isFlushed()){
                f->intInst.currentInstruction.reset(); // Flushing the pipleine
                f->setFlushed(0);
            }
            else
                f->intInst.currentInstruction = instruction;

            f->setMemAccessFinished(1);
            f->release->releaseEvent();
        } else if(pkt->getName() == "execute" && pkt->isRead()){        // only true for execute
            // Reading int from data memory
            if(!ex->getIsFloat()){
                int val = (*(int *)(pkt->getBuffer())); // Loading into rd. The store stage will get the data and store it in the proper location
                cout << "Loaded value: " << val << endl << endl;

                if(ex->intInst.funct3.to_string() == "101")        // LBH Zero Extend
                    val = val << 16;
                else if(ex->intInst.funct3.to_string() == "100")   // LBU Zero extend
                    val = val << 24;

                ex->intInst.rd.setData(val);
            }
            // Reading float from memory
            else{
                cout << "Loaded fp value: " << *(float *)(pkt->getBuffer()) << endl << endl;
                ex->fInst.rd.setData(*(float *)(pkt->getBuffer()));
            }

            ex->setMemAccessFinished(1); // Setting execute stage to not busy
        } else {
            if(!ex->getIsFloat())
                cout << "Successfully Stored " << *(int *)(pkt->getBuffer()) << " to Memory" << endl << endl;
            else{
                cout << "Successfully Stored " << *(float *)(pkt->getBuffer()) << " to Memory" << endl << endl;
                output.push_back(*(float *)(pkt->getBuffer()));
            }

            ex->setMemAccessFinished(1); // Setting execute stage to not busy
        }
        delete pkt;
}
