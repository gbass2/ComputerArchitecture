#include "simobject.hh"
#include <fstream>
#include <sstream>

using namespace std;

// Overhaul pipeline stages for assignment 3

// Fetching the instruction and putting it in the Pipeline class. Also printing it out.
void CPU::Fetch::fetchInstruction() { // Retrives the instruction from the instruction queue at the current program counter. Needs restructuring once Instructions class is implemented
}

// Prints the decode stage
void CPU::Decode::decodeInstruction() {
}

// Prints the execute stage
void CPU::Execute::executeInstruction() {

}

// Prints the store instruction
void CPU::Store::storeInstruction() {

}

// Main function for running the simulation
void RunSim::runSimulation(){
    clk = 10; // How far in advance that the event is going to be scheduled

    while(!(sys->getMEQ()).empty()){
        printMEQ();
        if ((sys->getMEQ().front()->getTime()) < currTick()){
            cout << "Error: Event was scheduled prior to the current time" << endl;
            assert(0);
        }

        // Schedules accesses the store stage
        if(!(strcmp(sys->getMEQ().front()->description(), "Store")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // s->storeInstruction();
            // sys->removeEvent();

        // Schedules and accesses the execute stage
        } if(!(strcmp(sys->getMEQ().front()->description(), "Execute")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // ex->executeInstruction();
            // s->setpipelineOpcode(ex->getExecuteStage().getpipelineOpcode());
            // s->setRegisters(ex->getExecuteStage().getRegisters());
            // s->process();

        // Schedules and access the decode stage
    } if(!(strcmp(sys->getMEQ().front()->description(), "Decode")) && (sys->getMEQ().front()->getTime()) == currTick()){
            // d->decodeInstruction();
            // ex->setpipelineOpcode(d->getDecodeStage().getpipelineOpcode());
            // ex->setRegisters(d->getDecodeStage().getRegisters());
            // ex->process();

        // Schedules and access the fetch stage
    } if(!(strcmp(sys->getMEQ().front()->description(), "Fetch")) && (sys->getMEQ().front()->getTime()) == currTick()){
        // f->fetchInstruction();
        // d->setpipelineOpcode(f->getFetchStage().getpipelineOpcode());
        // d->setRegisters(f->getFetchStage().getRegisters());

        // Scheduling decode for next instruction
        // d->process();
        // Scheduling fetch for next instruction
        // f->process();

        // Reading from file and placing the instructions in memeory
    } if(!(strcmp(sys->getMEQ().front()->description(), "Stall")) && (sys->getMEQ().front()->getTime()) == currTick()){

    } if(!(strcmp(sys->getMEQ().front()->description(), "Setup Simulation")) && (sys->getMEQ().front()->getTime()) == currTick()){
        setupSimulator(); // load the instructions into memory
    }





        incTick(1); // Increments currentTick by amount (t)
    }
}

void RunSim::setupSimulator(){
    ifstream inFile("CPU0.s"); //open the file set label as "inFile"
    vector<vector<string>>  wordIn; // multi dementional vector to hold the file's content

    //check the file is opened correctly
    if(!inFile){
        cout << "Unable to open the file: " << endl;
        exit(1);
    }

     // Fetching the instruction
     // Trying to read word by word.
     // The line read is added to an nXn vector wordIn to store the contents of the file
     string line;
     vector<string> tokens; // Holds the contents of the current line being read by the file
     int countDashesAtStart = 0;

     while(getline(inFile, line)){
         istringstream ls(line);
         string token; // The current word read from the file
         int count = 0;

         // Loops until the last line of the file
        while ( ls >> token ){
            // Removes comments
            if(token == "#")
                break;

            // // Removes the function name
            // if(token.back() == ':')
            //     break;

            // Saving the locations to branch back to



            if(token.back() == ','){ // Removes commas
                int p = token.find(",");
                token.erase(p);
            }
            tokens.push_back(token);
            count++;
         }

         // Used only for testing
         // Adds the current line without the comments and commas to a 2d vector wordIn
         if (tokens.size())		// Skip empty lines
             wordIn.emplace_back(move(tokens));
    }

    inFile.close();

    for ( const auto &row : wordIn ){
        for ( const auto &s : row ) std::cout << s << ' ';
        std::cout << std::endl;
    }
}
