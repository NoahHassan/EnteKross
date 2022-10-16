#pragma once

#include "Window.h"
#include <vector>

#include "Particle.h"

class App
{
public:
	App();
	App(const App&) = delete;
	App& operator =(const App&) = delete;
public:
	void Setup();
	void Update(float dt);
	void Draw();
private:
	Window wnd;
	Graphics& gfx;
private:
	std::vector<Particle> particles;
};