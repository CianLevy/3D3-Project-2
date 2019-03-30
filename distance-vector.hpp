#pragma once
#include <vector>
#include <cstdint> //uint
#include <string>
/*
    Class overview:
    This class encapsulates the distance vectors and their data. It will be used by the router to add all the data required to advertise a new link
    to a vector (buildDV) which can be sent to the other routers. 
    When a router receives a new link advertisement the data will be extracted and parsed by this class

    Note: the implementation of this class will be very similar to the datagram class
*/

class distance_vector {
    private:
        char sourceID;
        char destID;
        uint8_t cost;
        std::string sourceIP;
        uint16_t sourcePort;

    public:
        //Getters and setters for all the fields

        distance_vector(std::vector<uint8_t> buffer); //Construct a distance vector from a vector containing the relevant fields
        distance_vector(char sourceID, char destID, uint8_t cost, std::string sourceIP, uint16_t port); //Construct a distance vector by passing the relevant values
        std::vector<uint8_t> buildDV(); //Add the private attributes to a vector and return the vector
};