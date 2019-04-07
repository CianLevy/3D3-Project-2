#include <iostream>
#include <stdlib.h>     //srand, rand
#include <time.h>       //time
#include "datagram.hpp"
#include "router.hpp"
#include "router.cpp"

#define DEFAULT_TOPOLOGY "resources/links.csv"

void readUserInput();
void launchRouter(char ID, std::string topologyCSV);
void generateTraffic(char sourceID, char destID, std::string topologyCSV);
router* runningRouter;
std::thread* routerThread;

//Program entry point. The provided argument will specify the file name of any data packets being sent
int main(int argc, char* argv[]){

    if (argc == 2){
        if (argv[1][0] == 'r'){ //Generate random traffic
            srand (time(NULL));
            char sourceID = rand() % 26 + 65;
            char destID = sourceID;

            while (destID == sourceID)
                destID = rand() % 26 + 65;

            std::string p = "Example payload";
            std::vector<uint8_t> payload(p.begin(), p.end());
            
            datagram d(datagram::type::data, 'Z', destID, 'Z', payload); //Generated traffic will be flagged as coming from router Z
            router r(sourceID, 1, DEFAULT_TOPOLOGY);
            r.singleSend(d);
            std::cout << "Sent datagram" << std::endl;
        }
        else if (!std::strcmp(argv[1], "exit") && runningRouter != nullptr){
            runningRouter->close();
            routerThread->join();
            delete runningRouter;
        }
        else{
            std::cout << "Launching router " << argv[1][0] << " with default topology." << std::endl;
            launchRouter(argv[1][0], DEFAULT_TOPOLOGY);
        }

    }
    else if (argc == 3){
        std::cout << "Launching router " << argv[1][0] << " with topology specified by: " << argv[2] << std::endl;
        launchRouter(argv[1][0], argv[2]);
    }
    else if (argc == 4){
        if (argv[1][0] == 'g'){ //./my-router g A B: send traffic from A to B  
            generateTraffic(argv[2][0], argv[3][0], DEFAULT_TOPOLOGY);
        }
    }
    else if (argc == 5){
        if (argv[1][0] == 'g'){ //./my-router g A B example.csv: Use the topology in example.csv and send from A to B 
            generateTraffic(argv[2][0], argv[3][0], argv[4]);
        }
    }
    else{
        std::cout << "Invalid arguments" << std::endl;
    }  

}


void readUserInput(){
    std::string input = "";

    while (input != "exit"){
        std::cin >> input;
    }
    runningRouter->close();
    routerThread->join();
    delete runningRouter;

    std::cout << "Router closed" << std::endl;

}

void launchRouter(char ID, std::string topologyCSV){
    runningRouter = new router(ID, 0, topologyCSV);
    routerThread = new std::thread(&router::run, runningRouter);
    
    readUserInput();
}

void generateTraffic(char sourceID, char destID, std::string topologyCSV){
    std::string p = "Example payload";
    std::vector<uint8_t> payload(p.begin(), p.end());

    datagram d(datagram::type::data, 'Z', destID, 'Z', payload); //Generated traffic will be flagged as coming from router Z
    router r(sourceID, 1, topologyCSV);
    r.singleSend(d);
}