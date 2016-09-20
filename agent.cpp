#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

#define MAX_MAP_SIZE 40

Agent::Agent(int random_seed): orientation(NORTH){
	//supplying your own seed may help debugging, same seed will cause 
	//same random number sequence 
	if (random_seed == 0) std::srand(static_cast<unsigned>(std::time(0))); // random seed from time
	else                std::srand(random_seed); // random seed from user

	counter = 50;

	Node node;

	for (int i = 0; i < MAX_MAP_SIZE; ++i){
		std::vector<Node> row(MAX_MAP_SIZE);
		
		for (int j = 0; j < MAX_MAP_SIZE; ++j){
			node.pos.first = i;
			node.pos.second = j;
			row[j] = node;
		}
		roomMap.push_back(row);
	}

	homeRow = homeColumn = currentRow = currentColumn = MAX_MAP_SIZE / 2 - 1;
	roomMap[homeRow][homeColumn].status = NodeStatus::Discovered;

	tree.push(&roomMap[homeRow][homeColumn]);
}

Action Agent::GetAction(Percept p) {

	if (--counter == 0)
		return SHUTOFF;

	if (p.dirt){
		return SUCK;
	}

	if (p.bump){
		return LEFT;
	}

	return FORWARD;
}

