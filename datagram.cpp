#include "datagram.hpp"
#include <iostream>

datagram::datagram(std::vector<uint8_t> d){
    //Default values:
    sourceID = 0;
    t = type::data;
    length = 0;

    try {
        t = type(d.at(0)); //First bit is the type
        std::string header(d.begin(), d.begin() + (d.size() < 10 ? d.size() : 10)); //Assume the header is less than 10 bytes

        std::size_t found = header.find("\n");
        header = header.substr(found); //Extract the remaining half of the header
        found = header.find("\r\n");

        sourceID = (header.length() > 0 && found != std::string::npos) ? header.at(found - 1) : 0;
        length = d.size();
    }
    catch (std::exception& e) {
        std::cerr << "Exception occured parsing the datagram: " << e.what() << "\n";
    }
}

datagram::datagram(type t, char sourceID, std::vector<uint8_t> payload){
    std::string header = std::to_string(t) + "\n" + sourceID + "\r\n";

    packet.insert(packet.begin(), header.begin(), header.end());
    packet.insert(packet.end(), payload.begin(), payload.end());
    length = packet.size();
}