#include "distance-vector.hpp"

distance_vector::distance_vector(std::vector<uint8_t> buffer){
    sourceID = (char)buffer.at(0);
    destID = (char)buffer.at(1);
    sourceIP = (char)buffer.at(2);
    std::vector<unit8_t>temp.insert(temp.begin(), buffer.begin() + 3, buffer.at(4));
    cost = temp.size();
    std::vector<uint8_t>temp2.insert(temp2.begin(), buffer.begin() + 3 + cost, buffer.end());
    sourePort = temp2.size(); 
}
distance_vector::distance_vector(char sourceID, char destID, std::vector<uint8_t> cost, std::string sourceIP, uint16_t port){
    sourceID = sourceID;
    destID = destID;
    cost = cost;
    sourceIP = sourceIP;
    port = port;
}
std::vector<uint8_t> distance_vector::buildDV(){
    std::vector<uint8_t> dv;
    dv.insert(dv.begin(), &sourceID, &sourceID + sizeof(char));
    dv.insert(dv.end(), &destID, &destID + sizeof(char));
    dv.insert(dv.end(), &sourceIP,sourceIP.begin(), sourceIP.end());
    dv.insert(dv.end(), &cost, &cost + cost.size());
    dv.insert(dv.end(), &sourcePort, &sourcePort + sourcePort.size());
    return dv;
}