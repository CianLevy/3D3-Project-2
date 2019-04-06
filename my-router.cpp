#include <iostream>
#include <stdlib.h>     //srand, rand
#include <time.h>       //time
#include "datagram.hpp"
#include "router.hpp"
#include "router.cpp"


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
            router r(sourceID, 1);
            r.singleSend(d);
        }
        else{
            std::cout << "Launching router " << argv[1][0] << std::endl;
            router r(argv[1][0], 0);
            r.run();
        }

    }
    else if (argc == 4){
        if (argv[1][0] == 'g'){ //./my-router g A B: send traffic from A to B  
            std::string p = "Example payload";
            std::vector<uint8_t> payload(p.begin(), p.end());
            
            datagram d(datagram::type::data, 'Z', argv[3][0], 'Z', payload); //Generated traffic will be flagged as coming from router Z
            router r(argv[2][0], 1);
            r.singleSend(d);
        }
        }
    
    else{
        std::cout << "Invalid arguments" << std::endl;
    }  

}
