#include "Simulation.h"

// DEFAULTS

const float CELL_LINK_LENGTH = 1.0f;
const float REPULSION_FACTOR = 1.0f;
const float REPULSION_RANGE = 1.0f;
const float SPRING_FACTOR = 1.0f;
const float PLANAR_FACTOR = 1.0f;
const float BULGE_FACTOR = 1.0f;


Simulation::Simulation(size_t maxSize):
	m_cells(std::vector<Cell>()),
	m_positions(std::vector<GLfloat>(maxSize*3))
{
	m_cells.reserve(maxSize);
	m_maxSize = maxSize;
	m_settings = std::make_shared<SimulationParams>();

	// Settings setup
	m_settings->cellLinkLength = CELL_LINK_LENGTH;
	m_settings->repulsionFactor = REPULSION_FACTOR;
	m_settings->repulsionRange = REPULSION_RANGE;
	m_settings->springFactor = SPRING_FACTOR;
	m_settings->planarFactor = PLANAR_FACTOR;
	m_settings->bulgeFactor = BULGE_FACTOR;

	buildStartingCells();
}

Simulation::~Simulation()
{
	m_settings.reset();
}

void Simulation::update()
{
	//TODO
	
	// For each cell in cells:
	// increase energy -> split if energy is high enough
	// applyConnectionForces
	// applyGlobalRepulsionForces
	updatePositionsVector();
}

void Simulation::splitCell(Cell& cell)
{
	//TODO

	// choose 2 connected cells to determine plane of split
	// move all connections right to the split plane to the daughter cell
	// connect the two cells on the split plane to the daughter cell as well
}

int Simulation::getCellCount()
{
	return static_cast<int>(m_cells.size());
}

void Simulation::applyGlobalRepulsiveInfluences()
{
	//TODO

	// if 2 cells that are not connected are closer than repulsionRange,
	// move them away from each other by the repulsionFactor
}

void Simulation::applyConnectionForces(Cell& cell)
{
	glm::vec3 pos = cell.position;
	glm::vec3 bulgeTarget = calculateBulgeTarget(cell) - pos;
	glm::vec3 planarTarget = calculatePlanarTarget(cell) - pos;
	glm::vec3 springTarget = calculateSpringTarget(cell) - pos;

	cell.position = pos + (m_settings->springFactor * springTarget) + (m_settings->planarFactor * planarTarget) + (m_settings->bulgeFactor * bulgeTarget);
}

glm::vec3 Simulation::calculateBulgeTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

void Simulation::updatePositionsVector()
{
	// reset positions
	m_positions.clear();

	for (const Cell& cell : m_cells)
	{
		m_positions.emplace_back(cell.position.x);
		m_positions.emplace_back(cell.position.y);
		m_positions.emplace_back(cell.position.z);
	}
}

glm::vec3 Simulation::calculatePlanarTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

void Simulation::buildStartingCells()
{
	Cell cell;
	cell.position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cells.emplace_back(cell);

	Cell cell2;
	cell2.position = glm::vec3(2.0f, 0.0f, 0.0f);
	m_cells.emplace_back(cell2);

	cell.connectedCells.push_back(cell2);
	cell2.connectedCells.push_back(cell);
	updatePositionsVector();
}

glm::vec3 Simulation::calculateSpringTarget(Cell& cell)
{
	//TODO
	glm::vec3 target;
	return target;
}

std::vector<GLfloat>* Simulation::getPositionsVector()
{
	return &m_positions;
}
