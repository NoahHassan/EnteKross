#include "Particle.h"

Particle::Particle(float mass, Vec3 pos)
	:
	mass(mass),
	pos(pos),
	vel(Vec3::Zero())
{}

Particle::Particle(float mass, Vec3 pos, Vec3 vel)
	:
	mass(mass),
	pos(pos),
	vel(vel)
{}

void Particle::Draw(Graphics& gfx)
{
	gfx.DrawAt(pos.x, pos.y, pos.z);
}

void Particle::Update(float dt)
{
	vel += (force * (1.0f / mass)) * dt;
	pos += vel * dt;
}

void Particle::AddForce(Vec3 force)
{
	this->force += force;
}

void Particle::ResetForces()
{
	force = Vec3::Zero();
}

Vec3 Particle::GetPos()
{
	return pos;
}