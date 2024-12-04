#include "Simulation.h"

// DEFAULTS

const float CELL_LINK_LENGTH = 2.0f;
const float REPULSION_FACTOR = 1.0f;
const float REPULSION_RANGE = 1.0f;
const float SPRING_FACTOR = 1.5f;
const float PLANAR_FACTOR = 0.1f;
const float BULGE_FACTOR = 0.0001f;


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

	for (Cell& cell : m_cells)
	{
		cell.energy += 0.1f;
		if (cell.energy >= 1.0f)
		{
			splitCell(cell);
		}
		if (cell.connectedCells.size() > 0)
		{
			applyConnectionForces(cell);
		}
	}
	//applyGlobalRepulsiveInfluences();
	updatePositionsVector();
}

void Simulation::splitCell(Cell& cell)
{
	//TODO

	// choose 2 connected cells to determine plane of split
	// move all connections right to the split plane to the daughter cell
	// connect the two cells on the split plane to the daughter cell as well
	// reset mother cell energy to 0
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

	cell.position += (m_settings->springFactor * springTarget) + (m_settings->planarFactor * planarTarget) + (m_settings->bulgeFactor * bulgeTarget);
}

glm::vec3 Simulation::calculateBulgeTarget(Cell& cell)
{
	glm::vec3 target = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);

	if (glm::length(cell.position) > 0)
	{
		normal = glm::normalize(cell.position);
	}

	for (Cell& linkedCell : cell.connectedCells)
	{
		float dotN = glm::dot((linkedCell.position - cell.position), normal);

		float linkRestLengthSq = m_settings->cellLinkLength * m_settings->cellLinkLength;
		float linkedLen = glm::length(linkedCell.position);
		float linkedLenSq = linkedLen * linkedLen;

		target += sqrtf(linkRestLengthSq - linkedLenSq + (dotN*dotN) + dotN);
	}
	float factor = static_cast<float>(1.0f / cell.connectedCells.size());

	return cell.position + ((target * factor) * normal);
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
	glm::vec3 target = glm::vec3(0.0f);

	for (Cell& linkedCell : cell.connectedCells)
	{
		target += linkedCell.position;
	}

	return target * static_cast<float>(1.0f / cell.connectedCells.size());
}

void Simulation::buildStartingCells()
{
	Cell cell;
	cell.position = glm::vec3(-1.0f, 0.0f, 0.0f);
	cell.energy = 0.0f;
	cell.connectedCells = std::vector < Cell >();

	Cell cell2;
	cell2.position = glm::vec3(1.0f, 0.0f, 0.0f);
	cell2.energy = 0.0f;
	cell2.connectedCells = std::vector < Cell >();

	Cell cell3;
	cell3.position = glm::vec3(0.0f, 1.0f, -0.5f);
	cell3.energy = 0.0f;
	cell3.connectedCells = std::vector < Cell >();

	Cell cell4;
	cell4.position = glm::vec3(0.0f, 0.0f, -1.0f);
	cell4.energy = 0.0f;
	cell4.connectedCells = std::vector < Cell >();

	cell.connectedCells.push_back(cell2);
	cell.connectedCells.push_back(cell3);
	cell.connectedCells.push_back(cell4);

	cell2.connectedCells.push_back(cell);
	cell2.connectedCells.push_back(cell3);
	cell2.connectedCells.push_back(cell4);

	cell3.connectedCells.push_back(cell);
	cell3.connectedCells.push_back(cell2);
	cell3.connectedCells.push_back(cell4);

	cell4.connectedCells.push_back(cell);
	cell4.connectedCells.push_back(cell2);
	cell4.connectedCells.push_back(cell3);

	m_cells.emplace_back(cell);
	m_cells.emplace_back(cell2);
	m_cells.emplace_back(cell3);
	m_cells.emplace_back(cell4);

	updatePositionsVector();
}

glm::vec3 Simulation::calculateSpringTarget(Cell& cell)
{
	glm::vec3 target = glm::vec3(0.0f);

	for (Cell& linkedCell : cell.connectedCells)
	{
		target += linkedCell.position + (m_settings->cellLinkLength * glm::normalize(cell.position - linkedCell.position));
	}

	return target * static_cast<float>(1.0f / cell.connectedCells.size());
}

std::vector<GLfloat>* Simulation::getPositionsVector()
{
	return &m_positions;
}
