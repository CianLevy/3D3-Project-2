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
	std::string time = asctime(ti);


	if (file.is_open()){
		file << time.substr(0, time.size() -1) << ": <DV Update>" << std::endl;
		insertDVUpdate(cause);

		if (previousDV != nullptr){
			file << "Previous DV state:" << std::endl;
			insertDVinFile(*previousDV);
		}

		file << "Current DV state" << std::endl;
		insertDVinFile(currentDV);
		file << "</DV Update>" << std::endl;
		file << std::endl;
		
		previousDV = new distance_vector(currentDV);
	}
}

void logger::insertDVinFile(distance_vector d){
	file << std::setw(15) << std::left << "Destination" << 
			std::setw(15) << std::left << "Next router" << 
			std::setw(15) << std::left << "Port" <<
			std::setw(15) << std::left << "Cost" << std::endl;

	for (std::size_t i = 0; i < d.getCurrentDV().size(); i++){
		if ((d.getCurrentDV().at(i)).cost != 0xFF){


			file << std::setw(15) << std::left << (char)(i + 65) <<  
					std::setw(15) << std::left << (d.getCurrentDV().at(i)).nextHopID << 
					std::setw(15) << std::left << (d.getCurrentDV().at(i)).port << 
					std::setw(15) << std::left << (int)((d.getCurrentDV().at(i)).cost) <<  std::endl;
		}
	}
}

void logger::insertDVUpdate(struct dv_update d){
	file << "DV update from " << d.sourceID << std::endl;
	file << std::setw(15) << std::left << "Destination" <<
			std::setw(15) << std::left << "Cost" << std::endl;
	
	for (std::size_t i = 0; i < d.costs.size(); i++){
		if (d.costs.at(i) != 0xFF){
			file << std::setw(15) << std::left << (char)(i + 65) <<
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