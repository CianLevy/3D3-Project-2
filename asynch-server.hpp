#include <boost/asio.hpp>
#include <vector>
#include "router.hpp"

#define BUFFERLENGTH 100 //Brief recommends a maximum packet length of 100 bytes

class asynch_server{
    private:
        udp::socket socket_;
        std::vector<uint8_t> buffer;
        router r;

    public:
        void receive();
        asynch_server(uint16_t port);

};