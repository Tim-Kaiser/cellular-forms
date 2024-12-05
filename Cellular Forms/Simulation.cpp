#include "Simulation.h"

// DEFAULTS

const float CELL_LINK_LENGTH = 1.5f;
const float REPULSION_FACTOR = 1.0f;
const float REPULSION_RANGE = 1.0f;
const float SPRING_FACTOR = 1.5f;
const float PLANAR_FACTOR = 0.5f;
const float BULGE_FACTOR = 0.001f;


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
	
	// For each cell in cells:
	// increase energy -> split if energy is high enough
	// applyConnectionForces
	// applyGlobalRepulsionForces

	for (std::shared_ptr<Cell> cell : m_cells)
	{
		cell->energy += 0.01f;
		if (cell->energy >= 1.0f && m_cells.size() < 2000)
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

	// if 2 or less connected cells theres no need to determine the split plane 
	// because all connected cells will connect to both mother and daughter cell
	if (cell->connectedCells.size() <= 2)
	{
		std::shared_ptr<Cell> daughterCell = std::make_shared<Cell>();;
		daughterCell->energy = 0;
		daughterCell->position = cell->position + 0.1f;

		for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
		{
			daughterCell->connectedCells.push_back(linkedCell);
			linkedCell->connectedCells.push_back(daughterCell);

		}
		daughterCell->connectedCells.push_back(cell);
		cell->connectedCells.push_back(daughterCell);
		m_cells.emplace_back(daughterCell);
	}
	else
	{
		std::shared_ptr<Cell> daughterCell = std::make_shared<Cell>();;
		daughterCell->energy = 0;
		daughterCell->position = cell->position + 0.1f;

		// choose 2 connected cells to determine plane of split
		// move all connections right to the split plane to the daughter cell
		// connect the two cells on the split plane to the daughter cell as well
		// reset mother cell energy to 0
		
		// shuffle to (hopefully) avoid picking 2 elements that are next to each other
		std::shuffle(std::begin(cell->connectedCells), std::end(cell->connectedCells), m_rng);
		
		std::shared_ptr<Cell> cellA = cell->connectedCells[0];
		std::shared_ptr<Cell> cellB = cell->connectedCells[1];

		glm::vec3 A = cellA->position;
		glm::vec3 B = cellB->position;
		glm::vec3 normal = glm::cross(A, B);

		glm::vec3 splitPlane = A - B;

		unsigned i = 0;
		std::vector<unsigned int> indicesToRemove;
		indicesToRemove.reserve(cell->connectedCells.size());
		// maybe do this with a queue instead?
		for (std::shared_ptr<Cell> linkedCell : cell->connectedCells)
		{
			if (glm::dot( glm::cross(linkedCell->position, splitPlane), normal ) > 0.0f)
			{
				daughterCell->connectedCells.push_back(linkedCell);
				indicesToRemove.emplace_back(i);
			}
			++i;
		}

		i = 0;
		for (unsigned index : indicesToRemove)
		{
			//unsigned indexToRemove = indicesToRemove[j] - j;
			cell->connectedCells.erase(cell->connectedCells.begin() + (index - i));
			++i;
		}

		daughterCell->connectedCells.push_back(cellA);
		daughterCell->connectedCells.push_back(cellB);

		cellA->connectedCells.push_back(daughterCell);
		cellB->connectedCells.push_back(daughterCell);

		daughterCell->connectedCells.push_back(cell);
		cell->connectedCells.push_back(daughterCell);

		m_cells.emplace_back(daughterCell);
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

	//  + (m_settings->bulgeFactor * bulgeTarget)
	cell->position += (m_settings->springFactor * springTarget) + (m_settings->planarFactor * planarTarget);
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
