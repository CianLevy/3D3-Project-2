#pragma once
#include <vector>
#include <cstdint> //uint
#include <string>
#include <iomanip> //setw
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define DVLENGTH 26 //26 characters in the alphabet
#define INITIAL_LINKS "resources/links.csv"
#define CONVERTID(id) (id - 65) //IDs are capital letters and must be mapped to ints starting from 0
#define DEBUG false


/*
    Class overview:
    This class encapsulates the distance vectors and their data. It will be used by the router to add all the data required to advertise a new link
    to a vector (buildDV) which can be sent to the other routers. 
    When a router receives a new link advertisement the data will be extracted and parsed by this class

    Note: the implementation of this class will be very similar to the datagram class
*/
struct link
 {
    char nextHopID; //ID of the router the link connects to
    std::string ip; //Link router's IP
    uint16_t port; //Link router's listening port
    uint8_t cost; //Link cost 
};

struct dv_update{
    char sourceID;
    std::vector<uint8_t> costs;
};

class distance_vector {
    private:
        char routerID;
        std::vector<struct link> currentDV; //The actual distance vector. Node ids correspond to their location in the vector where the current cost is stored
        std::vector<struct link> immediateNeighbours;
        struct link buildLink(char routerID_, std::string ip_, uint16_t port_, uint8_t cost_);
        uint16_t listenPort;
        

    public:
        distance_vector(char ID);
        distance_vector(const distance_vector &old); //Copy constructor

        bool updateDV(dv_update d);
        void readLinkCosts();
        struct dv_update buildDVUpdate(std::vector<uint8_t> buffer);
        std::vector<uint8_t> getDVUpdate();
        void printDV();
        struct link getLink(char destID);
        void printForwardTable();
        void removeRouter(char ID);
        void restoreLink(char ID);

        std::vector<struct link> getCurrentDV(){return currentDV;};
        std::vector<struct link> getNeighbours(){ return immediateNeighbours; };

        uint16_t getListenPort(){ return listenPort; };
};