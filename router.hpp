#pragma once
#include <boost/asio.hpp>
#include "route-table.hpp"
#include "datagram.hpp"
#include "log.hpp"
#include "distance-vector.hpp"
#include <iostream>
#include <string>
#include "route-table.cpp"
#include "distance-vector.cpp"
#include "log.cpp"

#define BUFFERLENGTH 100
#define DEBUG true

using boost::asio::ip::udp;

/*
    Class overview:
    This class contains the server which receives incoming datagrams, processes them, and either updates the table or forwards the datagram to the correct router.
    The router must also periodically broadcast its current link costs to it's neighbours.
    The majority of the router's functionality will be built into the other classes. 
*/

class router{
    private:
        route_table table;
        logger* l;
        udp::socket *socket_;
        udp::endpoint server_endpoint;
        std::vector<uint8_t> buffer;
        router* r;
        bool stop;
        boost::asio::io_context io_context;
        char routerID;
        std::string ip;
        uint16_t port;

    public:
        void forwardDatagram(datagram d); //Forward the datagram to the correct link based on the values in the table
        void singleSend(datagram d); //Send a single datagram and close the server
        route_table getTable(){ return table; };
        void receive();
        router(uint16_t port_); //Initialise the router with a specified listening port
        void run(); //Launch the router and wait for connections
        void broadcast(datagram d); //Function to send a datagram to all of router's immediate neighbours
};