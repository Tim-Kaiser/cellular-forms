#include "Simulation.h"

// DEFAULTS

const float CELL_LINK_LENGTH = 1.0f;
const float REPULSION_FACTOR = 1.0f;
const float REPULSION_RANGE = 1.0f;
const float SPRING_FACTOR = 1.0f;
const float PLANAR_FACTOR = 1.0f;
const float BULGE_FACTOR = 1.0f;

SimulationParams* settings = nullptr;

void setupSimulation(std::vector<Cell>& cells, int maxSize)
{
	cells.reserve(static_cast<rsize_t>(maxSize));
	// TODO starting cells
}

void splitCell(Cell& cell)
{

}

void applyGlobalRepulsiveInfluences(std::vector<Cell>& cells)
{
	//TODO
}

void applyConnectionForces(Cell& cell)
{
	glm::vec3 pos = cell.position;
	glm::vec3 bulgeTarget = calculateBulgeTarget(cell) - pos;
	glm::vec3 planarTarget = calculatePlanarTarget(cell) - pos;
	glm::vec3 springTarget = calculateSpringTarget(cell) - pos;

	cell.position = pos + (settings->springFactor * springTarget) + (settings->planarFactor * planarTarget) + (settings->bulgeFactor * bulgeTarget);
}

glm::vec3 calculateBulgeTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

glm::vec3 calculatePlanarTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

glm::vec3 calculateSpringTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

void fillPositionsVector(std::vector<Cell>& cells, std::vector<GLfloat>& positions)
{
	// reset positions
	positions.clear();

	for (const Cell& cell : cells)
	{
		positions.emplace_back(cell.position.x);
		positions.emplace_back(cell.position.y);
		positions.emplace_back(cell.position.z);
	}
}

SimulationParams::SimulationParams(int maxSize, float repulsionFactor = REPULSION_FACTOR, float repulsionRange = REPULSION_RANGE, float springFactor = SPRING_FACTOR, float planarFactor = PLANAR_FACTOR, float bulgeFactor = BULGE_FACTOR, float cellLinkLength = CELL_LINK_LENGTH) :
	maxSize(maxSize),
	repulsionFactor(repulsionFactor),
	repulsionRange(repulsionRange),
	springFactor(springFactor),
	planarFactor(planarFactor),
	bulgeFactor(bulgeFactor),
	cellLinkLength(cellLinkLength)
{
}
