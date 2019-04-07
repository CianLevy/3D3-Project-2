#include "datagram.hpp"

datagram::datagram(std::vector<uint8_t> d, uint8_t length_){
    d.shrink_to_fit();
    t = (type)d.at(0);
    sourceID = (char)d.at(1);
    destID = (char)d.at(2);
    previousHopID = (char)d.at(3);
    payload.insert(payload.begin(), d.begin() + 4, d.begin() + length_);
    length = length_;

    #if DEBUG
        std::cout << "Buffer: ";
        for (uint8_t t : d)
            std::cout << (int) t << " ";
        std::cout << "\nBuffer length" << d.size() << "\n\n\n";

        std::cout << "Payload: ";
        for (uint8_t t : payload)
            std::cout << (int) t << " ";
        std::cout << "\nPayload length" << payload.size() << "\n\n\n";
    #endif
}

datagram::datagram(type t_, char sourceID_, char destID_, char previousHopID_, std::vector<uint8_t> payload_){
    t = t_;
    sourceID = sourceID_;
    payload = payload_;
    destID = destID_;
    previousHopID = previousHopID_;
    payload.shrink_to_fit();
    length = payload.size() + 3; 
}

std::vector<uint8_t> datagram::buildDatagram(){
    std::vector<uint8_t> d;
    char typeState = (char)(t & 0xFF);
    d.insert(d.begin(), &typeState, &typeState + sizeof(char));
    d.insert(d.end(), &sourceID, &sourceID + sizeof(char));
    d.insert(d.end(), &destID, &destID + sizeof(char));
    d.insert(d.end(), &previousHopID, &previousHopID + sizeof(char));
    d.insert(d.end(), payload.begin(), payload.end());
    return d;
}

datagram::~datagram(){
    payload.clear();
}