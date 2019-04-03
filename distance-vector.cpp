#define _CRT_SECURE_NO_WARNINGS
#define MAXDIST 9999
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "distance-vector.hpp"

using namespace std;

distance_vector::distance_vector(std::vector<uint8_t> buffer) {
	sourceID = (char)buffer.at(0);
	destID = (char)buffer.at(1);
	sourceIP = (char)buffer.at(2);
	std::vector<uint8_t>temp.insert(temp.begin(), buffer.begin() + 3, buffer.at(4));
	cost = temp.size();
	std::vector<uint8_t>temp2.insert(temp2.begin(), buffer.begin() + 3 + cost, buffer.end());
	sourcePort = temp2.size();
}
distance_vector::distance_vector(char sourceID, char destID, std::vector<uint8_t> cost, std::string sourceIP, uint16_t port) {
	sourceID = sourceID;
	destID = destID;
	cost = cost;
	sourceIP = sourceIP;
	port = port;
}


std::vector<uint8_t> distance_vector::buildDV() {
	std::vector<uint8_t> dv;
	dv.insert(dv.begin(), &sourceID, &sourceID + sizeof(char));
	dv.insert(dv.end(), &destID, &destID + sizeof(char));
	dv.insert(dv.end(), &sourceIP, sourceIP.begin(), sourceIP.end());
	dv.insert(dv.end(), &cost, &cost + cost.size());
	dv.insert(dv.end(), &sourcePort, &sourcePort + sourcePort.size());
	return dv;
}

// I've never worked with vectors before so I believe I'm making a few mistakes I'll have to fix.
// This code doesn't compile due to the errors I've made with handling the vectors, but the 
// minimisation distance vector algorithm does work.
struct Graph {
	//std::vector<uint8_t> 
	struct distance_vector* dv;
	int num_nodes;
	int num_connections;
};


//struct connection {
//	struct distance_vector sourceID, destID;
//int cost;
//};



struct Graph* createGraph(int number_connections) {

	struct Graph* graph1 = (struct Graph*) malloc(sizeof(struct Graph));
	
	graph1->num_connections = number_connections;
	graph1->dv = (struct distance_vector*) malloc( number_connections * sizeof(struct distance_vector));
	
	return graph1;
}


void DV_algo(struct Graph* graph, int source, int num_connections, int num_nodes) {
	//int num_nodes = 6;
	int shortest_distance[6];


	for (int i = 0; i < 6; i++) { shortest_distance[i] = MAXDIST; }
	shortest_distance[source] = 0;

	//The shortest path of graph that contain V vertices, never contain "V-1" edges. So we do here "V-1" relaxations
	for (int i = 1; i <= 6 - 1; i++){

		for (int j = 0; j < num_connections; j++){
			//formula adapted fromhttps://cseweb.ucsd.edu/classes/fa11/cse123-a/123f11_Lec9.pdf
			

			int u = graph->dv[j].sourceID;
			int v = graph->dv[j].destID;
			int cost = graph->dv[j].cost;

			if (shortest_distance[u] + cost < shortest_distance[v])
				shortest_distance[v] = shortest_distance[u] + cost;
		}
	}
}


int get_num_connections() {
	
	ifstream infile;
	infile.open("links.csv");
	if (!infile) {
		cerr << "Error opening 'links.csv'";
		exit(1);
	}

	int connections = 0;
	string a;
	ifstream infile("links.csv");

	while (getline(infile, a)) {connections++;}
	infile.close();
	return connections;

}


int main() {

	char node1, node2;
	int port_num, cost, num_nodes = 6;

	int num_connections = get_num_connections();

	struct Graph* graph1 = createGraph(num_connections);    

	ifstream infile;
	infile.open("links.csv");
	if (!infile) {
		cerr << "Error opening 'links.csv'";
		exit(1);   
	}

	int i = 0;
	while (!infile.eof()) {

		infile >> node1;
		infile >> node2;
		infile >> port_num;
		infile >> cost;
		i++;
		graph1->dv[0].buildDV( node1,  node2, cost, 0 ,  port_num);


		if (i > num_connections) {
			cout << "Error in file";
			exit(1);
		}
	}
	infile.close();

	int source = 0;
	DV_algo(graph1, source, num_nodes, num_connections);

	return 0;
}
