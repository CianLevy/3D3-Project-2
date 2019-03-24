#include <string>
#include <fstream>
#include "route-table.hpp"

class log{
    private:
         std::ofstream fileStream;

    public:
        log(std::string dir);
        void tableUpdate(route_table rt); //When the table is updated this must be relected in the log
}