#include "log.hpp"


logger::logger(char ID_){
	ID = ID_;
	std::string dir = SAVE_DIR;
	dir[19] = ID_;
	file.open(dir);
	previousDV = nullptr;
}
void logger::recordDVUpdate(distance_vector currentDV, struct dv_update cause){
	time_t tt;
	struct tm * ti;
	time (&tt); 
	ti = localtime(&tt); 


	if (file.is_open()){
		file << "<DV Update>" << std::endl;
		file << addIndentation(INDENT) << "Time: " << asctime(ti);
		insertDVUpdate(cause, 2 * INDENT);

		if (previousDV != nullptr){
			file << addIndentation(INDENT) << "Previous DV state:" << std::endl;
			insertDVinFile(*previousDV, 2 * INDENT);
		}

		file << addIndentation(INDENT) << "Current DV state:" << std::endl;
		insertDVinFile(currentDV, 2 * INDENT);
		file << "</DV Update>" << std::endl;
		file << std::endl;
		
		previousDV = new distance_vector(currentDV);
	}
}

void logger::insertDVinFile(distance_vector d, int indent){
	if (file.is_open()){
		file << addIndentation(indent) << std::setw(15) << std::left << "Destination" << 
				std::setw(15) << std::left << "Next router" << 
				std::setw(15) << std::left << "Port" <<
				std::setw(15) << std::left << "Cost" << std::endl;

		for (std::size_t i = 0; i < d.getCurrentDV().size(); i++){
			if ((d.getCurrentDV().at(i)).cost != 0xFF){


				file << addIndentation(indent) << std::setw(15) << std::left << (char)(i + 65) <<  
						std::setw(15) << std::left << (d.getCurrentDV().at(i)).nextHopID << 
						std::setw(15) << std::left << (int)(d.getCurrentDV().at(i)).port << 
						std::setw(15) << std::left << (int)((d.getCurrentDV().at(i)).cost) <<  std::endl;
			}
		}
		file << std::endl;
	}
}

void logger::recordRoutedDatagram(datagram d, uint16_t arrivalPort, uint16_t departPort){
	time_t tt;
	struct tm * ti;
	time (&tt); 
	ti = localtime(&tt); 


	if (file.is_open()){
		file << "<Routed Packet>" << std::endl;
		file << addIndentation(INDENT) << "Time: " << asctime(ti);
		file << addIndentation(INDENT) << "The Packet Source ID is " << d.getID() << std::endl;
		file << addIndentation(INDENT) << "The previous hop ID is " << d.getPreviousHopID() << std::endl;	
		file << addIndentation(INDENT) << "The Destination ID is " << d.getDestID() << std::endl;
		file << addIndentation(INDENT) << "Arrival UDP Port is " << (int)(arrivalPort) << std::endl;
		file << addIndentation(INDENT) << "The Forward Port is " << (int)(departPort) << std::endl;


		if (ID == d.getDestID()){
			/* Printing the entire payload */
			file << addIndentation(INDENT) << "The Packet Payload contains: " << std::endl;
			file << addIndentation(INDENT);
			for(std::size_t i = 0; i < d.getPayload().size(); i++){
				file << (char)(d.getPayload().at(i));
			}
			file << std::endl;
		}
		file << "</Routed Packet>" << std::endl;
		file << std::endl;
	}
}

void logger::insertDVUpdate(struct dv_update d, int indent){
	file << addIndentation(indent / 2) << "DV update from " << d.sourceID << ":" << std::endl;
	file << addIndentation(indent) << std::setw(15) << std::left << "Destination" <<
			std::setw(15) << std::left << "Cost" << std::endl;
	
	for (std::size_t i = 0; i < d.costs.size(); i++){
		if (d.costs.at(i) != 0xFF){
			file << addIndentation(indent) << std::setw(15) << std::left << (char)(i + 65) <<
					std::setw(15) << std::left << (int)d.costs.at(i) << std::endl;
		}
	}
	file << std::endl;
}

void logger::recordInitialDV(distance_vector currentDV){
	time_t tt;
	struct tm * ti;
	time (&tt); 
	ti = localtime(&tt); 

	previousDV = new distance_vector(currentDV);

	if (file.is_open()){
		file << "<Initialised DV>" << std::endl;
		file << addIndentation(INDENT) << "Time: " << asctime(ti);
		insertDVinFile(currentDV, 2 * INDENT);

		file << "</Initialised DV>" << std::endl;
		file << std::endl;
	}
}

std::string logger::addIndentation(int indentDepth){
	std::string s = "";

	for (int i = 0; i < indentDepth; i++)
		s += " ";

	return s;
}

void logger::recordRouterDropout(distance_vector currentDV, char deadID){
	time_t tt;	//Note should probably just make this a class variable
	struct tm * ti;
	time (&tt); 
	ti = localtime(&tt); 

	if (file.is_open()){
		file << "<Router Dropout>" << std::endl;
		file << addIndentation(INDENT) << "Time: " << asctime(ti);
		file << addIndentation(INDENT) << "Router " << deadID << " has not responded and is considered dead." << std::endl;

		if (previousDV != nullptr){
			file << addIndentation(INDENT) << "Previous DV state:" << std::endl;
			insertDVinFile(*previousDV, 2 * INDENT);
		}

		file << addIndentation(INDENT) << "Current DV state:" << std::endl;
		insertDVinFile(currentDV, 2 * INDENT);
		previousDV = new distance_vector(currentDV);
		file << "</Router Dropout>" << std::endl;
		file << std::endl;
	}
}

void logger::recordRouterJoin(distance_vector currentDV, char joinedID){
	time_t tt;
	struct tm * ti;
	time (&tt); 
	ti = localtime(&tt); 

	if (file.is_open()){
		file << "<Router Joined>" << std::endl;
		file << addIndentation(INDENT) << "Time: " << asctime(ti);
		file << addIndentation(INDENT) << "Router " << joinedID << " has connected." << std::endl;

		if (previousDV != nullptr){
			file << addIndentation(INDENT) << "Previous DV state:" << std::endl;
			insertDVinFile(*previousDV, 2 * INDENT);
		}

		file << addIndentation(INDENT) << "Current DV state:" << std::endl;
		insertDVinFile(currentDV, 2 * INDENT);
		previousDV = new distance_vector(currentDV);
		file << "</Router Joined>" << std::endl;
		file << std::endl;
	}
}