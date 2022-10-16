#include "Particle.h"

Particle::Particle(float x, float y, float z)
	:
	x(x),
	y(y),
	z(z)
{}

void Particle::Draw(Graphics& gfx)
{
	gfx.DrawAt(x, y, z);
}