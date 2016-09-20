#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <vector>
#include <stack>

typedef struct Node Node;

typedef std::vector<std::vector<Node>> Map;

struct Position{
	int x, y;
	Position(int newX = 0, int newY = 0) : x(newX), y(newY){};
	bool operator==(Position const & rhs){ return (x == rhs.x && y == rhs.y); }
	
};

enum AgentState{
	RUNNING,
	CHANGING_BRANCH,
	TURNING_LEFT,
	TURNING_RIGHT,
	TURNING_BACK,
	DONE
};

enum NodeStatus{
	UNDISCOVERED,
	DISCOVERED,
	WALL
};

struct Node{
	Position pos;
	NodeStatus status;
	bool isWall;
	int childCounter = 0;
	std::vector<Node*> children;
	Node* parent = NULL;
	Node() :pos(0, 0), status(UNDISCOVERED), isWall(false){}

	// FUNCTIONS
	Heading RelativeOrientation(Node* node);
};

class Agent {
public:
	Agent(int random_seed);
	Action GetAction(Percept p);
private:
	Node* homeNode;
	Node* currentNode;
	Node* nextTarget;
	Map roomMap;
	std::stack<Node*> undiscoveredNodes;
	Heading orientation; // will be North by default, I'm going to perceive the map from roomba's perspective
	AgentState state;
	// FUNCTIONS
	void GetCoordinatesInDirection(Node& currentNode, Heading requestedDirection, Position& coordinates);
	void DiscoverNode(Node& nodeToBeDiscovered);
	void RegisterChild(Node& parentNode, Position& childPosition);
	Node* GetChildNodeInOrientation(Heading orientation);
	Node* GetNextAvailableChild();
	Action OrientTowardsTarget();
	Action AdvanceTowardsNextNode();

};

#endif

