#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include "route-table.hpp"
#include "datagram.hpp"
#include "datagram.cpp"

/*
    Class overview:
    This class will be responsible for recording: changes to the routing table, datagrams received and routed.
    The log will be a file called "routing-outputX.txt" (where X is the router ID) which will be stored in the "resources" folder.
    Suggested implementation:
        The brief specifies the following must be stored when the routing table is updated:
            -   The current time --
            -   The table before the update --
            -   DV which caused the changed (including the source ID)
            -   The new routing table
        Storing the state of the table prior to an update requires that the previous state is stored somewhere.
        I suggest that the previous table state should be stored in "previousTable" among the private attributes in this class.
        When recordTableUpdate is called, "recordTableUpdate" will be one update behind. After the required records are written to the file
        previousTable can be overwritten with the current table state.



        The brief also specifies that the following must be recorded when a packet is routed:
            -   ID of the packet source
            -   The packet's destination
            -   Arrival UDP port
            -   UDP port the packet was forwarded to
            -   The packet contents (only at the destination router)
        This functionaility is simpler and wont need any state information.
*/

class logger{
    private:
        std::fstream fileStream;
        std::string routeID;
        route_table previousTable;

    public:
        logger(char ID); //When constructing the logger and opening the log, only the router ID is required to fully specify the file name
        void recordTableUpdate(route_table currentTable); //All the information required to record the table update is provided by "previousTable" and currentTable
                                                         //ctime will have to be used to record the time of the update
        void recordRoutedDatagram(datagram d, uint16_t arrivalPort, uint16_t departPort); //The majority of the required information is stored in the datagram class
        std::string getrouteID(){ return routeID; } //Returns the log name of the route ID. Doesn't return the actual route ID
};
