#pragma once
#include <glm.hpp>
#include <vector>
#include "include/glad/gl.h"


struct SimulationParams
{
	int maxSize;
	float repulsionFactor;
	float repulsionRange;
	float springFactor;
	float planarFactor;
	float bulgeFactor;
	float cellLinkLength;

	SimulationParams(int maxSize, float repulsionFactor, float repulsionRange, float springFactor, float planarFactor, float bulgeFactor, float cellLinkLength);
};


struct Cell
{
	glm::vec3 position;
	float energy;
	std::vector<Cell> connectedCells;
};


void applyConnectionForces(Cell &cell);
void applyGlobalRepulsiveInfluences(std::vector<Cell> &cells);
void splitCell(Cell &cell);
void setupSimulation(std::vector<Cell> &cells, int maxSize);

void fillPositionsVector(std::vector<Cell>& cells, std::vector<GLfloat>& positions);

glm::vec3 calculateSpringTarget(Cell& cell);
glm::vec3 calculatePlanarTarget(Cell& cell);
glm::vec3 calculateBulgeTarget(Cell& cell);