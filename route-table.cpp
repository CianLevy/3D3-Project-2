#include "route-table.hpp"


struct link route_table::getLink(char destID){
    return table.at(destID % 64); //This assumes the IDs are A - Z
}
int route_table::updateTable(distance_vector d){
    /*
    Implement the DV algorithm: When router a router advertises a new link, check the existing link cost. If the new link is cheaper update the 
    table.
    Return a flag (e.g. 0x7FFFFFFF) if there are multiple updates. This will cause all links to be advertised again
    If there are no updates return -1. If only one link was updated return the location of the link in the vector
    */
}  
