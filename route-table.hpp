#pragma once
#include <vector>
#include <string>
#include "distance-vector.hpp"

/*
    Class overview:
    This class effectively implements a dynamically updated lookup table. When the router receives a datagram
    whose destination is X, the link in position X in the "table" vector will specify which router the datagram
    should be forwarded to based on the current least cost path.
    When a new link is advertised the path cost is calculated based on the values in the vector. If the new cost
    is less than the current cost the link will be updated


*/

class route_table {
    private:
        std::vector<struct link> table; //The location in the vector will specify the destination
        //E.g. traffic directed to router A will be forwarded to the link in position 0 in the vector
        void readLinkCosts(); //This function will read the link costs to the router's neighbours from the CSV and construct the initial routing table 
                                //This function should be called by the constructor.
    
    public:
        std::vector<struct link> getTable() { return table; };
        struct link getLink(char destID); //Return the link the packet should be forwarded to to reach the destination ID
        int updateTable(distance_vector d);  //For an explanation of this function's behaviour see the cpp
};


struct link {
    char routerID; //ID of the router the link connects to
    std::string ip; //Link router's IP
    uint16_t port; //Link router's listening port
    uint8_t cost; //Link cost 
};
