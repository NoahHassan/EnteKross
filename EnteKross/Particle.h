#pragma once

#include "Graphics.h"
#include "Vec3.h"

class Particle
{
public:
	Particle(float mass, Vec3 pos);
	Particle(float mass, Vec3 pos, Vec3 vel);
public:
	void Draw(Graphics& gfx);
	void Update(float dt);
	void AddForce(Vec3 force);
	void ResetForces();
	Vec3 GetPos();
private:
	float mass;
	Vec3 pos;
	Vec3 vel;
	Vec3 force = Vec3::Zero();
};