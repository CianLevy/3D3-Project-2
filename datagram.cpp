#include "datagram.hpp"
#include <iostream>

datagram::datagram(std::vector<uint8_t> d){
    t = (type)d.at(0);
    sourceID = (char)d.at(1);
    payload.insert(payload.begin(), d.begin() + 2, d.end());
    length = payload.size();

}

datagram::datagram(type t_, char sourceID_, std::vector<uint8_t> payload_){
    t = t_;
    sourceID = sourceID_;
    payload = payload_;
    length = payload.size() + 2;  
}

std::vector<uint8_t> datagram::buildDatagram(){
    std::vector<uint8_t> d;
    char typeState = (char)(t & 0xFF);
    d.insert(d.begin(), &typeState, &typeState + sizeof(char));
    d.insert(d.end(), &sourceID, &sourceID + sizeof(char));
    d.insert(d.end(), &payload[0], &payload[0] + payload.size());
    return d;
}