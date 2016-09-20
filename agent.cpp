#include <ctime> //for random seed
#include <cmath> 
#include <iostream>
#include "agent.h"

#define MAX_MAP_SIZE 40
#define MAX_CHILD_COUNTER 4

Agent::Agent(int random_seed): orientation(NORTH), state(RUNNING), nextTarget(NULL), homeNode(NULL), currentNode(NULL){
	//supplying your own seed may help debugging, same seed will cause 
	//same random number sequence 
	if (random_seed == 0) std::srand(static_cast<unsigned>(std::time(0))); // random seed from time
	else                std::srand(random_seed); // random seed from user

	Node node;

	for (int i = 0; i < MAX_MAP_SIZE; ++i){
		std::vector<Node> row(MAX_MAP_SIZE);
		
		for (int j = 0; j < MAX_MAP_SIZE; ++j){
			node.pos.x = i;
			node.pos.y = j;
			row[j] = node;
		}
		roomMap.push_back(row);
	}

	currentNode = homeNode = &roomMap[MAX_MAP_SIZE / 2 - 1][MAX_MAP_SIZE / 2 - 1];
	DiscoverNode(*homeNode);
}

Action Agent::GetAction(Percept p) {

	if (p.home){ //End condition
		if (state == DONE)
			return SHUTOFF;
	}
	
	if (p.dirt){
		return SUCK;
	}
	else{

		if (p.bump){ // Hit a wall
			nextTarget->status = WALL;
			nextTarget = NULL;
			return AdvanceTowardsNextNode();
		}

		Node* childNode = GetChildNodeInOrientation(orientation); // Get the child in my direction
		if (childNode){
			nextTarget = childNode;
			DiscoverNode(*nextTarget);
			return FORWARD;
		}
		else{
			return AdvanceTowardsNextNode();
		}

		

	}
}

void Agent::GetCoordinatesInDirection(Node& currentNode, Heading requestedDirection, Position& coordinates)
{
	switch (requestedDirection)
	{
	case NORTH: //If I want to go to North, coordinates = (x, y-1)
		coordinates.x = currentNode.pos.x;
		coordinates.y = currentNode.pos.y - 1;
		break;
	case EAST: // If East, (x+1, y)
		coordinates.x = currentNode.pos.x + 1;
		coordinates.y = currentNode.pos.y;
		break;
	case SOUTH: // If South, (x, y+1)
		coordinates.x = currentNode.pos.x;
		coordinates.y = currentNode.pos.y + 1;
		break;
	case WEST: // If West, (x-1, y)
		coordinates.x = currentNode.pos.x - 1;
		coordinates.y = currentNode.pos.y;
		break;
	}
}

void Agent::DiscoverNode(Node& nodeToBeDiscovered)
{
	Position childrenCoordinates;

	for (int i = 0; i < 4; ++i){
		GetCoordinatesInDirection(nodeToBeDiscovered, (Heading)i, childrenCoordinates);
		RegisterChild(nodeToBeDiscovered, childrenCoordinates);
	}

	nodeToBeDiscovered.status = DISCOVERED;
}

void Agent::RegisterChild(Node& parentNode, Position& childPosition)
{
	Node* childNode;
	childNode = &roomMap[childPosition.x][childPosition.y];
	childNode->parent = &parentNode;
	parentNode.children.push_back(childNode);
}


Node* Agent::GetChildNodeInOrientation(Heading orientation)
{
	Node* childNode = currentNode->children[orientation];
	if (childNode->status != UNDISCOVERED)
		return NULL;

	return childNode;
}

Node* Agent::GetNextAvailableChild()
{
	Node* childNode = NULL;
	int counter = 0;
	do 
	{
		childNode = GetChildNodeInOrientation((Heading)counter++);
	} while (childNode->status != UNDISCOVERED || counter != MAX_CHILD_COUNTER);

	return childNode;


}

Action Agent::OrientTowardsTarget()
{
	Heading leftSide = static_cast<Heading>((orientation + 3) % 4);
	Position
	

	Heading rightSide = static_cast<Heading>((orientation + 1) % 4);


	Heading backSide = static_cast<Heading>((orientation + 2) % 4);
}


Action Agent::AdvanceTowardsNextNode()
{
	if (!nextTarget){ // Find the next child
		nextTarget = GetNextAvailableChild();
	}

	if (!nextTarget){ // No available child -> go up one level
		TurnBack();
	}
	else{
		DiscoverNode(*nextTarget);
		return OrientTowardsTarget();
	}


	/*switch (state)
	{
	case RUNNING:
		Node* childNode = GetNextAvailableChild(); //Get the next available child;
		if (childNode){ // If child is found, advance towards that child's direction
			OrientAndAdvance(childNode);
		}
		else{ // If not then this node is completely discovered, move to parent
			currentNode = *currentNode.parent;
		}
	case TURNING_AROUND:
		break;
	case RETURN_TO_NEXT_BRANCH:
		break;
	case DONE:
		break;
	default:
		break;
	}*/
	
}


Action Agent::TurnBack()
{

}

Heading Node::RelativeOrientation(Node* node)
{
	Position relativeNodePosition = node->pos;
	int deltaX = pos.x - relativeNodePosition.x;
	if (deltaX < 0){
		return WEST;
	}
	else if (deltaX > 0){
		return EAST;
	}

	int deltaY = pos.y - relativeNodePosition.y;
	if (deltaY < 0){
		return NORTH;
	}

	return SOUTH;
}
