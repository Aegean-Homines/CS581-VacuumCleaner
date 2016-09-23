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
	homeNode->parent = NULL;
	DiscoverNode(*homeNode);
	homeNode->status = DISCOVERED;
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
			currentNode->status = WALL;
			currentNode->children.clear();
			nextTarget = NULL;
			currentNode = currentNode->parent;
		}

		return AdvanceTowardsNextNode();

		/*Node* childNode = GetChildNodeInOrientation(orientation); // Get the child in my direction
		if (childNode){
			nextTarget = childNode;
			DiscoverNode(*nextTarget);
			return FORWARD;
		}
		else{
			return AdvanceTowardsNextNode();
		}*/

		

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
	if (nodeToBeDiscovered.status != UNDISCOVERED)
		return;

	for (int i = 0; i < MAX_CHILD_COUNTER; ++i){
		Position childrenCoordinates;
		GetCoordinatesInDirection(nodeToBeDiscovered, (Heading)i, childrenCoordinates);
		if (nodeToBeDiscovered.parent != NULL && nodeToBeDiscovered.parent->pos == childrenCoordinates){
			continue;
		}
		RegisterChild(nodeToBeDiscovered, childrenCoordinates);
	}
}

void Agent::RegisterChild(Node& parentNode, Position& childPosition)
{
	Node* childNode;
	childNode = &roomMap[childPosition.x][childPosition.y];
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

	for (int i = 0; i < currentNode->children.size() && !childNode; ++i){
		childNode = GetChildNodeInOrientation((Heading)i);
	}

	return childNode;


}

Action Agent::OrientTowardsTarget()
{
	Heading targetOrientation = currentNode->RelativeOrientation(nextTarget);
	if (targetOrientation == orientation){
		state = RUNNING;
		if (nextTarget->status == UNDISCOVERED){
			nextTarget->parent = currentNode;
			nextTarget->status = DISCOVERED;
		}
		currentNode = nextTarget;
		nextTarget = NULL;
		return FORWARD;
	}
	Heading leftSide = static_cast<Heading>((orientation + 3) % 4);
	if (targetOrientation == leftSide){
		state = TURNING_LEFT;
		orientation = leftSide;
		return LEFT;
	}
	Heading rightSide = static_cast<Heading>((orientation + 1) % 4);
	if (targetOrientation == rightSide){
		state = TURNING_RIGHT;
		orientation = rightSide;
		return RIGHT;
	}
	Heading backSide = static_cast<Heading>((orientation + 2) % 4);
	if (targetOrientation == backSide){
		state = TURNING_BACK;
		orientation = leftSide;
		return LEFT;
	}
}

Action Agent::AdvanceTowardsNextNode()
{
	if (!nextTarget){ // Find the next child
		/*nextTarget = GetChildNodeInOrientation(orientation);
		if (!nextTarget){
			nextTarget = GetNextAvailableChild();
		}*/
		nextTarget = GetNextAvailableChild();
	}

	if (!nextTarget){ // If no child is available

		if (currentNode->parent == NULL){
			return SHUTOFF;
		}//This condition returns true only for shutoff state
		nextTarget = currentNode->parent;
	}

	DiscoverNode(*nextTarget);
	return OrientTowardsTarget();
	
}
Heading Node::RelativeOrientation(Node* node)
{
	Position relativeNodePosition = node->pos;
	int deltaX = pos.x - relativeNodePosition.x;
	if (deltaX > 0){
		return WEST;
	}
	else if (deltaX < 0){
		return EAST;
	}

	int deltaY = pos.y - relativeNodePosition.y;
	if (deltaY > 0){
		return NORTH;
	}

	return SOUTH;
}
