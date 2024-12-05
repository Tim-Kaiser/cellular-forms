#pragma once
#include <glm.hpp>
#include <vector>
#include "include/glad/gl.h"
#include <memory>
#include <random>
#include <algorithm>

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
	std::vector<std::shared_ptr<Cell>> connectedCells;
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
	void applyConnectionForces(std::shared_ptr<Cell> cell);
	void applyGlobalRepulsiveInfluences();
	void splitCell(std::shared_ptr<Cell> cell);

	glm::vec3 calculateSpringTarget(std::shared_ptr<Cell> cell);
	glm::vec3 calculatePlanarTarget(std::shared_ptr<Cell> cell);
	glm::vec3 calculateBulgeTarget(std::shared_ptr<Cell> cell);

	void updatePositionsVector();

	std::shared_ptr<SimulationParams> m_settings;
	std::vector<std::shared_ptr<Cell>> m_cells;
	std::vector<GLfloat> m_positions;
	size_t m_maxSize;
	std::default_random_engine m_rng;
};



