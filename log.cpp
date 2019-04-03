#include "log.hpp"


logger::logger(char ID){
	std::string tempID = "route_ID";
	tempID += ID;
	routeID = tempID;
}

void logger::recordTableUpdate(route_table rt){
	std::string line; //Current line in the file

	fileStream.open(routeID); //Opening the specified file
	getline(fileStream, line);
	if(fileStream.is_open()){
		do{
			std::cout << line << '\n';
		} while(getline(fileStream, line));
	fileStream.close();
	}
	previousTable = rt;
	fileStream.open(routeID);//Or fileStreamO.open("route_ID", ios::app) so there's no overwriting.

	time_t update_time; //Time sampling
	update_time = time(NULL);
	char *tm = ctime(&update_time);
	fileStream << "Router updated at " << tm << std::endl;

	fileStream << rt << std::endl; //Writing to the file. This line doesn't work
	fileStream.close();
}

void logger::recordRoutedDatagram(datagram d, uint16_t arrivalPort, uint16_t departPort){
	std::ofstream loggerFile;
	std::vector<uint8_t> pay = d.getPayload();
	uint8_t payLength = d.getLength();
	char sourceID = d.getID();
	char destID = d.getDestID();
	loggerFile.open("log_file.txt", std::ios::app); //Not sure if we want a txt file to store all the logs or we want to cout or what
	loggerFile << "The Packet Source ID is " << sourceID << std::endl;
	loggerFile << "The Destination ID is " << destID << std::endl;
	loggerFile << "Arrival UDP Port is " << unsigned(arrivalPort) << std::endl;
	loggerFile << "The Forward Port is " << unsigned(departPort) << std::endl;

	//Printing the entire payload
	loggerFile << "The Packet Payload contains: " << std::endl;
	for(int i = 0; i <= payLength; i++){
		loggerFile << unsigned(pay[i]) << std::endl;
	}

	loggerFile.close();
}
