#include <iostream>
#include "datagram.hpp"
#include "datagram.cpp"
#include "router.hpp"
#include "router.cpp"




//Program entry point. The provided argument will specify the file name of any data packets being sent
int main(int argc, char* argv[]){

    if (argc > 1){
        std::cout << argv[1] << std::endl;
        if (std::stoi(argv[1]) == -1){
            std::string p = "Example payload";
            std::vector<uint8_t> p_vector(p.begin(), p.end());
            datagram d(datagram::type::control, 'A', p_vector);
            router r(10004);
            r.singleSend(d);
        }

    }
    else{
        router r(10000);
        r.run();
    }  
}