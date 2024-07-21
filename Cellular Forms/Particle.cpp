#include "Particle.h"

Particle::Particle(int x, int y, int z)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_size = 1;
	m_energy = 0.0;
}

void Particle::update()
{
}

void Particle::addNeighbor(Particle &p)
{
	m_neighbors.push_back(p);
}

void Particle::calculateNewPosition()
{
}
