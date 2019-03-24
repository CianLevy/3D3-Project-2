#include "route-table.hpp"
#include "datagram.hpp"
#include "log.hpp"

class router {
    private:
        route_table table;
        log l;

    public:
        void forwardDatagram(char destID, datagram d); //Forward the datagram to the correct link based on the values in the table
        route_table getTable(){ return table; };
        
};