#include "distance-vector.hpp"

using namespace std;
distance_vector::distance_vector(char ID){
	routerID = ID;

	for (int i = 0; i < DVLENGTH; i++){
        struct link l;
		if (i == CONVERTID(routerID))
			l = buildLink(routerID, "127.0.0.1", 0xFFFF, 0); //The cost for a router to get to itself is 0
		else
			l = buildLink(0xFF, "127.0.0.1", 0xFFFF, 0xFF); //Initialise as a flagged null link
        currentDV.push_back(l);
    }
	readLinkCosts();
}

distance_vector::distance_vector(const distance_vector &old){
	routerID = old.routerID;
	listenPort = old.listenPort;

	currentDV.insert(currentDV.begin(), old.currentDV.begin(), old.currentDV.end());
}

bool distance_vector::updateDV(dv_update d){
	//To do: handle link cost increases and router drop out
	bool update = false;
	int loopLength = (d.costs.size() < currentDV.size()) ? d.costs.size() : currentDV.size();

	for (int i = 0 ; i < loopLength; i++){
		if (currentDV.at(i).cost > currentDV.at(CONVERTID(d.sourceID)).cost + (d.costs).at(i)	//Option 1: the link is cheaper
			|| ((currentDV.at(i).cost == currentDV.at(CONVERTID(d.sourceID)).cost + (d.costs).at(i)) && d.sourceID < currentDV.at(i).nextHopID && (d.costs).at(i) != 0xFF)){	//Option 2: the link is the same cost but router ID is lower
			#if DEBUG
				std::cout << "Update from " << d.sourceID << std::endl;
				std::cout << "Previous cost to " << (char)(i + 65) << ": " << (int)currentDV.at(i).cost << std::endl;
				std::cout << "Updated next hop to " << d.sourceID << " with total cost: " << (int)(currentDV.at(CONVERTID(d.sourceID)).cost + (d.costs).at(i)) << std::endl;
				std::cout << "\n";
			#endif

			uint8_t previousCost = currentDV.at(i).cost;

			currentDV.at(i).cost = currentDV.at(CONVERTID(d.sourceID)).cost + (d.costs).at(i);
			currentDV.at(i).nextHopID = currentDV.at(CONVERTID(d.sourceID)).nextHopID; //Handles the case where a direct neighbour is no longer the preferred route to itself
			currentDV.at(i).ip = currentDV.at(CONVERTID(d.sourceID)).ip;
			currentDV.at(i).port = currentDV.at(CONVERTID(d.sourceID)).port;

			for (std::size_t j = 0 ; j < currentDV.size(); j++){
				if (currentDV.at(j).nextHopID  == (char)(i + 65)){   //if the next hop is the router which was just updated in the new link: update the cost with the new cost
					currentDV.at(j).cost += currentDV.at(i).cost - previousCost; //Remove the previous cost and update with the new cost
					currentDV.at(j).nextHopID = currentDV.at(CONVERTID(d.sourceID)).nextHopID;
					currentDV.at(j).ip = currentDV.at(CONVERTID(d.sourceID)).ip;
					currentDV.at(j).port = currentDV.at(CONVERTID(d.sourceID)).port;

					#if DEBUG
						std::cout << "Previous cost to " << (char)(j + 65) << ": " << (int)previousCost << std::endl;
						std::cout << "New total cost: " << (int)(currentDV.at(j).cost) << std::endl;
						std::cout << "\n";
					#endif
				}
			}
			update = true;
        }

	}
	return update;
}

void distance_vector::printDV(){
	std::cout << "Distance vector for router " << routerID << std::endl;
    std::cout <<  std::setw(15) << std::left << "Destination" << std::setw(15) << std::left <<  
    "Cost" <<  std::endl;

   	for (std::size_t i = 0; i < currentDV.size(); i++){
		if (currentDV.at(i).cost != 0xFF)	//Link is not flagged as not existing
			std::cout << std::setw(15) << std::left << (char)(i + 65)
						<<  std::setw(15) << std::left << (int)currentDV.at(i).cost << std::endl;

    }
}

void distance_vector::readLinkCosts(){
    
    std::ifstream infile;
	infile.open(INITIAL_LINKS);
	if (!infile) {
		std::cerr << "Error opening 'links.csv'";
		exit(1);   
	}
    
    char source, dest;
    uint16_t port;
    uint8_t cost;
    std::string ip = "127.0.0.1"; //Assume localhost
    std::string buffer;

	while (!infile.eof()) {
        std::getline(infile, buffer, ',');
        source = buffer[0];
        std::getline(infile, buffer, ',');
        dest = buffer[0];
        std::getline(infile, buffer, ',');
        port = std::stoi(buffer);
        std::getline(infile, buffer, '\n');
        cost = std::stoi(buffer);
        
        if (source == routerID)   //Only interested in immediate neighbours
            (currentDV.at(CONVERTID(dest))) = buildLink(dest, ip, port, cost);
		else if (dest == routerID){
			currentDV.at(CONVERTID(dest)).port = port;
			listenPort = port;
		}
	}
	infile.close();
}

struct link distance_vector::buildLink(char routerID_, std::string ip_, uint16_t port_, uint8_t cost_){
    struct link l;
    l.nextHopID = routerID_;
    l.ip = ip_;
    l.port = port_;
    l.cost = cost_;
    return l;
}

struct link distance_vector::getLink(char destID){
    return (currentDV.at(CONVERTID(destID)));
}

struct dv_update distance_vector::buildDVUpdate(std::vector<uint8_t> buffer){
	struct dv_update d;
	d.sourceID = (char)buffer.at(0);
	d.costs.insert(d.costs.begin(), buffer.begin() + 1, buffer.end());

	return d;
}

std::vector<uint8_t> distance_vector::getDVUpdate(){
	std::vector<uint8_t> temp;
	temp.push_back((uint8_t)routerID);

	for (struct link l : currentDV){
		temp.push_back(l.cost);
	}

	return temp;
}

void distance_vector::printForwardTable(){
    std::cout << "Forward table for " << routerID << std::endl;
    std::cout <<  std::setw(15) << std::left << "Destination" << std::setw(15) << std::left <<  
    "Next router" << std::setw(15) << std::left << "Port" <<
    std::setw(15) << std::left << "Cost" << std::endl;

    for (std::size_t i = 0; i < currentDV.size(); i++){
        if ((currentDV.at(i)).cost != 0xFF){
            std::cout <<  std::setw(15) << std::left << (char)(i + 65) <<  std::setw(15) << std::left
                        << (currentDV.at(i)).nextHopID << std::setw(15) << std::left
                        << (currentDV.at(i)).port << std::setw(15) << std::left
                        << (int)((currentDV.at(i)).cost) <<  std::endl;
        }
    }
}
