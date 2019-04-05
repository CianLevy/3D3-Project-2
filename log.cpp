#include "log.hpp"


logger::logger(char ID){
	std::string route_ID = "routing-output" << ID;
}

void logger::recordTableUpdate(route_table rt){
	//Reading whats in the file first
	fileStreamI.open("route_ID");
	std::string line; //Current line in the file
	getline(fileStreamI, line); //Loading the first line

	if(fileStreamI.is_open()){
		do{
			std::cout << line << '\n';
		  } while (getline(fileStreamI, line));
	fileStreamI.close();
	}

	//Writing to the file
	fileStreamO.open("route_ID");

	time_t update_time;
	update_time = time(NULL);
	char *tm = ctime(&update_time);
	fileStreamO << "Router updated at " << tm << std::endl;

	if(fileStreamO.is_open()){
		outputStream << rt << std::endl;
	}
}

void logger::recordRoutedDatagram(datagram d, uint16_t arrivalPort, uint16_t departPort){
	std::vector<uint8_t> pay = d.getPayload();
	uint8_t payLength = d.getLength(); /* Getting the size of the payload */
	char sourceID = d.getID();
	char destID = d.getDestID();
	fileStream.open("log_file.txt", std::ios::app); /* Opening a seperate log file to store all transfers */
	fileStream << "The Packet Source ID is " << sourceID << std::endl;
	fileStream << "The Destination ID is " << destID << std::endl;
	fileStream<< "Arrival UDP Port is " << unsigned(arrivalPort) << std::endl;
	fileStream << "The Forward Port is " << unsigned(departPort) << std::endl;

	/* Printing the entire payload */
	fileStream << "The Packet Payload contains: " << std::endl;
	for(int i = 0; i <= payLength; i++){
		fileStream << unsigned(pay[i]) << std::endl;
	}

	fileStream.close();

}
