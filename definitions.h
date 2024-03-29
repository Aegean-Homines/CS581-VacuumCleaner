#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <cstdlib>

struct Percept {
	bool dirt : 1;
	bool bump : 1;
	bool home : 1;
	//charge is not used
	size_t charge : 8 * sizeof(int*) - 3; //29 bits for 32-bit OS, 61 bit for 64-bit
};

enum Action { LEFT, RIGHT, FORWARD, SUCK, SHUTOFF, NOOP };

enum Heading { NORTH, EAST, SOUTH, WEST };

const size_t initial_charge = 100;
const size_t max_steps = 10000;

#endif
