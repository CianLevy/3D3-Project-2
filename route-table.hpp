#pragma once
#include <vector>
#include <string>

class route_table {
    private:
        std::vector<struct link> table; //The location in the vector will specify the destination
        //E.g. traffic directed to router A will be forwarded to the link in position 0 in the vector
    
    public:
        struct link getLink(char destID); //Return the link the packet should be forwarded to reach the destination ID
        bool updateTable(char sourceID, char destID, uint8_t cost, std::string sourceIP, uint16_t sourcePort);  
        //When router a router advertises a new link, check the existing link cost. If the new link is cheaper update the table and return true
};


struct link {
    char routerID; //ID of the router the link connects to
    std::string ip; //Link router's IP
    uint16_t port; //Link router's listening port
    uint8_t cost; //Link cost 
};