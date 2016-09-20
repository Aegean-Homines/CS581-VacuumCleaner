#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <vector>
#include <stack>
#include <unordered_map>

typedef struct Node Node;

typedef std::pair<int, int> Position;
typedef std::vector<std::vector<Node>> Map;

enum NodeStatus{
	OpenNode,
	Discovered
};

struct Node{
	Position pos;
	NodeStatus status;
	bool isWall;
	Node() :pos(0, 0), status(OpenNode), isWall(false){}
};

class Agent {
public:
	Agent(int random_seed);
	Action GetAction(Percept p);
private:
	Heading selfHeadingDirection;
	int counter;
	int homeRow, homeColumn, currentRow, currentColumn;
	Map roomMap;
	std::stack<Node*> tree;
	Heading orientation; // will be North by default, I'm going to perceive the map from roomba's perspective
};

#endif

