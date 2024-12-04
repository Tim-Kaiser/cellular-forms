#include "Simulation.h"

// DEFAULTS

const float CELL_LINK_LENGTH = 2.5f;
const float REPULSION_FACTOR = 1.0f;
const float REPULSION_RANGE = 1.0f;
const float SPRING_FACTOR = 1.5f;
const float PLANAR_FACTOR = 1.3f;
const float BULGE_FACTOR = 0.0001f;


Simulation::Simulation(size_t maxSize):
	m_cells(std::vector<std::shared_ptr<Cell>>()),
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

	for (std::shared_ptr<Cell> cell : m_cells)
	{
		cell->energy += 0.1f;
		if (cell->energy >= 1.0f)
		{
			splitCell(cell);
		}
		if (cell->connectedCells.size() > 0)
		{
			applyConnectionForces(cell);
		}
	}
	//applyGlobalRepulsiveInfluences();
	updatePositionsVector();
}

void Simulation::splitCell(std::shared_ptr<Cell> cell)
{
	// choose 2 connected cells to determine plane of split
	// move all connections right to the split plane to the daughter cell
	// connect the two cells on the split plane to the daughter cell as well
	// reset mother cell energy to 0


	// if 2 or less connected cells theres no need to determine the split plane 
	// because all connected cells will connect to both mother and daughter cell
	if (cell->connectedCells.size() <= 2)
	{
		std::shared_ptr<Cell> daughterCell = std::make_shared<Cell>();;
		daughterCell->energy = 0;
		daughterCell->position = cell->position;
		daughterCell->position.x += 0.1f;

		for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
		{
			daughterCell->connectedCells.push_back(linkedCell);
			linkedCell->connectedCells.push_back(daughterCell);

		}
		daughterCell->connectedCells.push_back(cell);
		m_cells.emplace_back(daughterCell);
	}
	else
	{

	}

	cell->energy = 0.0f;

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

void Simulation::applyConnectionForces(std::shared_ptr<Cell> cell)
{
	glm::vec3 pos = cell->position;
	glm::vec3 bulgeTarget = calculateBulgeTarget(cell) - pos;
	glm::vec3 planarTarget = calculatePlanarTarget(cell) - pos;
	glm::vec3 springTarget = calculateSpringTarget(cell) - pos;

	cell->position += (m_settings->springFactor * springTarget) + (m_settings->planarFactor * planarTarget) + (m_settings->bulgeFactor * bulgeTarget);
}

glm::vec3 Simulation::calculateBulgeTarget(std::shared_ptr<Cell> cell)
{
	glm::vec3 target = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);

	if (glm::length(cell->position) > 0)
	{
		normal = glm::normalize(cell->position);
	}

	for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
	{
		float dotN = glm::dot((linkedCell->position - cell->position), normal);

		float linkRestLengthSq = m_settings->cellLinkLength * m_settings->cellLinkLength;
		float linkedLen = glm::length(linkedCell->position);
		float linkedLenSq = linkedLen * linkedLen;

		target += sqrtf(linkRestLengthSq - linkedLenSq + (dotN*dotN) + dotN);
	}
	float factor = static_cast<float>(1.0f / cell->connectedCells.size());

	return cell->position + ((target * factor) * normal);
}

void Simulation::updatePositionsVector()
{
	// reset positions
	m_positions.clear();

	for (const std::shared_ptr<Cell> cell : m_cells)
	{
		m_positions.emplace_back(cell->position.x);
		m_positions.emplace_back(cell->position.y);
		m_positions.emplace_back(cell->position.z);
	}
}

glm::vec3 Simulation::calculatePlanarTarget(std::shared_ptr<Cell> cell)
{
	glm::vec3 target = glm::vec3(0.0f);

	for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
	{
		target += linkedCell->position;
	}

	return target * static_cast<float>(1.0f / cell->connectedCells.size());
}

void Simulation::buildStartingCells()
{
	std::shared_ptr<Cell> cell = std::make_shared<Cell>();
	cell->position = glm::vec3(-1.0f, 0.0f, 0.0f);
	cell->energy = 0.0f;
	cell->connectedCells = std::vector < std::shared_ptr<Cell> >();

	std::shared_ptr<Cell> cell2 = std::make_shared<Cell>();
	cell2->position = glm::vec3(1.0f, 0.0f, 0.0f);
	cell2->energy = 0.0f;
	cell2->connectedCells = std::vector < std::shared_ptr<Cell> >();

	std::shared_ptr<Cell> cell3 = std::make_shared<Cell>();
	cell3->position = glm::vec3(0.0f, 1.0f, -0.5f);
	cell3->energy = 0.0f;
	cell3->connectedCells = std::vector < std::shared_ptr<Cell> >();

	std::shared_ptr<Cell> cell4 = std::make_shared<Cell>();
	cell4->position = glm::vec3(0.0f, 0.0f, -1.0f);
	cell4->energy = 0.0f;
	cell4->connectedCells = std::vector < std::shared_ptr<Cell> >();

	cell->connectedCells.push_back(cell2);
	cell->connectedCells.push_back(cell3);
	//cell.connectedCells.push_back(cell4);

	cell2->connectedCells.push_back(cell);
	cell2->connectedCells.push_back(cell3);
	//cell2.connectedCells.push_back(cell4);

	cell3->connectedCells.push_back(cell);
	cell3->connectedCells.push_back(cell2);
	//cell3.connectedCells.push_back(cell4);

	cell4->connectedCells.push_back(cell);
	cell4->connectedCells.push_back(cell2);
	cell4->connectedCells.push_back(cell3);

	m_cells.emplace_back(cell);
	m_cells.emplace_back(cell2);
	m_cells.emplace_back(cell3);
	//m_cells.emplace_back(cell4);

	updatePositionsVector();
}

glm::vec3 Simulation::calculateSpringTarget(std::shared_ptr<Cell> cell)
{
	glm::vec3 target = glm::vec3(0.0f);

	for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
	{
		target += linkedCell->position + (m_settings->cellLinkLength * glm::normalize(cell->position - linkedCell->position));
	}

	return target * static_cast<float>(1.0f / cell->connectedCells.size());
}

std::vector<GLfloat>* Simulation::getPositionsVector()
{
	return &m_positions;
}
