#pragma once
#include <glm.hpp>
#include <vector>
#include "include/glad/gl.h"
#include <memory>

struct SimulationParams
{
	int maxSize;
	float repulsionFactor;
	float repulsionRange;
	float springFactor;
	float planarFactor;
	float bulgeFactor;
	float cellLinkLength;
};


struct Cell
{
	glm::vec3 position;
	float energy;
	std::vector<Cell> connectedCells;
};

class Simulation
{
public:
	int getCellCount();
	Simulation(size_t maxSize);
	~Simulation();

	void update();

	std::vector<GLfloat>* getPositionsVector();


private:
	void buildStartingCells();
	void applyConnectionForces(Cell& cell);
	void applyGlobalRepulsiveInfluences();
	void splitCell(Cell& cell);

	glm::vec3 calculateSpringTarget(Cell& cell);
	glm::vec3 calculatePlanarTarget(Cell& cell);
	glm::vec3 calculateBulgeTarget(Cell& cell);

	void updatePositionsVector();

	std::shared_ptr<SimulationParams> m_settings;
	std::vector<Cell> m_cells;
	std::vector<GLfloat> m_positions;
	size_t m_maxSize;
};



