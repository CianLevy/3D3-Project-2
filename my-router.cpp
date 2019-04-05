#include <iostream>
#include "datagram.hpp"
#include "router.hpp"
#include "router.cpp"


//Program entry point. The provided argument will specify the file name of any data packets being sent
int main(int argc, char* argv[]){

    if (argc > 1){

        if (argv[1][0] == '0'){
            std::string p = "Example payload";
            std::vector<uint8_t> p_vector(p.begin(), p.end());
            datagram d(datagram::type::data, 'A','B', p_vector);
            router r('A');
            r.singleSend(d);
        }
        else{
            std::cout << "Launching router " << argv[1][0] << std::endl;
            router r(argv[1][0]);
            r.run();
        }

    }
    else{
        std::cout << "Invalid arguments" << std::endl;
    }  

}