#pragma once

#include <vector>

class Particle {
public:
	Particle(int x, int y, int z);
	void update();
	void addNeighbor(Particle &p);

private:
	void calculateNewPosition();

	int m_x;
	int m_y;
	int m_z;
	int m_size;
	double m_energy;
	std::vector<Particle> m_neighbors;
};

