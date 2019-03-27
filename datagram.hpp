#pragma once
#include <vector>
#include <cstdint>
#include <string>

class datagram{
    


    public:
          
        enum type {control, data};
        datagram(type t, char sourceID, std::vector<uint8_t> payload);
        datagram(std::vector<uint8_t> d); //Demultiplex a received datagram and extract its payload

        std::vector<uint8_t> getPacket() { return packet; };
        char getType() { return t; };
        char getID() { return sourceID; };
        uint8_t getLength() { return length; };

      

    private:
        std::vector<uint8_t> packet;
        type t;
        char sourceID;
        uint8_t length;

};