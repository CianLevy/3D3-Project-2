#pragma once
#include <boost/asio.hpp>
#include "route-table.hpp"
#include "datagram.hpp"
#include "log.hpp"
#include <iostream>
#include <string>

#define BUFFERLENGTH 100
#define DEBUG true

using boost::asio::ip::udp;

class router{
    private:
        route_table table;
        //log l;#
        udp::socket *socket_;
        udp::endpoint server_endpoint;
        std::vector<uint8_t> buffer;
        router* r;
        bool stop;
        boost::asio::io_context io_context; 

    public:
        void forwardDatagram(datagram d); //Forward the datagram to the correct link based on the values in the table
        void singleSend(datagram d); //Send a single datagram and close the server
        route_table getTable(){ return table; };
        void receive();
        router(uint16_t port);
        router();
        void run();
};