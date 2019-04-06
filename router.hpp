#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>   //boost::bind
#include <iostream>
#include <string>
#include "datagram.hpp"
#include "distance-vector.hpp"
#include "distance-vector.cpp"
#include "log.hpp"
#include "log.cpp"

#define BUFFERLENGTH 1000
#define DEBUG false
#define MAXCONNECTIONS 26
#define REFRESHPERIOD 3 //Defines how frequently the distance vector is advertised
#define DROP_OUT_PERIOD_COUNT 2 //Number of consecutive periods without receiving an update from a neighbour required for the neighbour to be considered 'dead'

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
        std::vector<uint8_t>                timeSinceReceived;
        std::vector<bool>                   live;
        boost::asio::deadline_timer         timer;
        void periodicRetransmit();

    public:
        void forwardDatagram(datagram d); //Forward the datagram to the correct link based on the values in the table
        void singleSend(datagram d); //Send a single datagram and close the server
        void receive();
        router(char ID, bool mode); //Initialise the router with a specified id
        void run(); //Launch the router and wait for connections
        void broadcast(bool control, std::vector<uint8_t> payload); //Function to send a datagram to all of router's immediate neighbours
};