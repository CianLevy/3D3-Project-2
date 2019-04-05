#pragma once
#include <boost/asio.hpp>
#include "boost/bind.hpp" 
#include "datagram.hpp"
#include "log.hpp"
#include "distance-vector.hpp"
#include <iostream>
#include <string>
#include "distance-vector.cpp"
#include "log.cpp"

#define BUFFERLENGTH 100
#define DEBUG false
#define MAXCONNECTIONS 26
#define REFRESHPERIOD 1 //Defines how frequently the distance vector is advertised

using boost::asio::ip::udp;

/*
    Class overview:
    This class contains the server which receives incoming datagrams, processes them, and either updates the table or forwards the datagram to the correct router.
    The router must also periodically broadcast its current link costs to it's neighbours.
    The majority of the router's functionality will be built into the other classes. 
*/

class router{
    private:
        //To do: reformat and simplify. Use initialisation lists instead of pointers
        distance_vector*                    dv;
        logger*                             l;
        udp::socket*                        socket_;
        udp::endpoint                       server_endpoint;
        std::vector<uint8_t>                buffer;
        router*                             r;
        bool                                stop;
        boost::asio::io_context             io_context;
        char                                routerID;
        std::string                         ip;
        uint16_t                            port;
        uint8_t                             timeSinceReceived[MAXCONNECTIONS];
        bool                                received[MAXCONNECTIONS];
        boost::asio::deadline_timer         timer;

        void periodicRetransmit();

    public:
        void forwardDatagram(datagram d); //Forward the datagram to the correct link based on the values in the table
        void singleSend(datagram d); //Send a single datagram and close the server
        void receive();
        router(char ID); //Initialise the router with a specified id
        void run(); //Launch the router and wait for connections
        void broadcast(bool control, std::vector<uint8_t> payload); //Function to send a datagram to all of router's immediate neighbours
};