#include "log.hpp"


logger::logger(char ID){
	std::string dir = SAVE_DIR;
	dir[19] = ID;
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
		insertDVUpdate(cause);

		if (previousDV != nullptr){
			file << addIndentation(INDENT) << "Previous DV state:" << std::endl;
			insertDVinFile(*previousDV);
		}

		file << addIndentation(INDENT) << "Current DV state" << std::endl;
		insertDVinFile(currentDV);
		file << "</DV Update>" << std::endl;
		file << std::endl;
		
		previousDV = new distance_vector(currentDV);
	}
}

void logger::insertDVinFile(distance_vector d){
	file << addIndentation(INDENT) << std::setw(15) << std::left << "Destination" << 
			std::setw(15) << std::left << "Next router" << 
			std::setw(15) << std::left << "Port" <<
			std::setw(15) << std::left << "Cost" << std::endl;

	for (std::size_t i = 0; i < d.getCurrentDV().size(); i++){
		if ((d.getCurrentDV().at(i)).cost != 0xFF){


			file << addIndentation(INDENT) << std::setw(15) << std::left << (char)(i + 65) <<  
					std::setw(15) << std::left << (d.getCurrentDV().at(i)).nextHopID << 
					std::setw(15) << std::left << (d.getCurrentDV().at(i)).port << 
					std::setw(15) << std::left << (int)((d.getCurrentDV().at(i)).cost) <<  std::endl;
		}
	}
}

void logger::recordRoutedDatagram(datagram d, uint16_t arrivalPort, uint16_t departPort){
	std::vector<uint8_t> pay = d.getPayload();
	uint8_t payLength = d.getLength(); /* Getting the size of the payload */
	char sourceID = d.getID();
	char destID = d.getDestID();

	file << "<Routed Packet>" << std::endl;
	file << addIndentation(INDENT) << "The Packet Source ID is " << sourceID << std::endl;
	file << addIndentation(INDENT) << "The Destination ID is " << destID << std::endl;
	file << addIndentation(INDENT) << "Arrival UDP Port is " << unsigned(arrivalPort) << std::endl;
	file << addIndentation(INDENT) << "The Forward Port is " << unsigned(departPort) << std::endl;


	/* Printing the entire payload */
	file << addIndentation(INDENT) << "The Packet Payload contains: " << std::endl;
	for(int i = 0; i <= payLength; i++){
		file << addIndentation(INDENT) << unsigned(pay[i]) << std::endl;
	}

	file << "</Routed Packet>" << std::endl;
}

void logger::insertDVUpdate(struct dv_update d){
	file << addIndentation(INDENT) << "DV update from " << d.sourceID << std::endl;
	file << addIndentation(INDENT) << std::setw(15) << std::left << "Destination" <<
			std::setw(15) << std::left << "Cost" << std::endl;
	
	for (std::size_t i = 0; i < d.costs.size(); i++){
		if (d.costs.at(i) != 0xFF){
			file << addIndentation(INDENT) << std::setw(15) << std::left << (char)(i + 65) <<
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
	std::string time = asctime(ti);


	if (file.is_open()){
		file << time.substr(0, time.size() -1) << ": <Initialised DV>" << std::endl;
		insertDVinFile(currentDV);

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