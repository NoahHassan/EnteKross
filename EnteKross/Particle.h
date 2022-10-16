#pragma once

#include "Graphics.h"

class Particle
{
public:
	Particle(float x, float y, float z);
public:
	void Draw(Graphics& gfx);
public:
	float x;
	float y;
	float z;
};