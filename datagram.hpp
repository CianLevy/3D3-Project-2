#pragma once
#include <vector>
#include <cstdint>
#include <string>
/*
    Class overview:
    This class encapsulates the datagrams which are transmitted between routers. It is used by the router class to both construct and deconstruct datagrams.
    From the sender point of view the class will be used to multiplex the payload data and datagram header.
    From the receiver point of view the class will be used to demultiplex the payload data and header fields.

    Note: The header currently contains only the data type and source ID. Other fields will most likely need to be added.


*/


class datagram{

    public:
          
        enum type {data, control};
        datagram(type t_, char sourceID_, std::vector<uint8_t> payload_);
        datagram(std::vector<uint8_t> d); //Demultiplex a received datagram and extract its payload

        std::vector<uint8_t> buildDatagram();

        std::vector<uint8_t> getPayload() { return payload; };
        char getType() { return t; };
        char getID() { return sourceID; };
        uint8_t getLength() { return length; };

    private:
        std::vector<uint8_t> payload;
        type t;
        char sourceID;
        char destID;

        uint8_t length;

};