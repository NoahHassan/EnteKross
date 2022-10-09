#include "App.h"

#include <cmath>

App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
}

void App::Update(float dt)
{
	t += dt;
	c = (1.0f + std::sin(t)) / 2.0f;
}

void App::Draw()
{
	gfx.BeginFrame(c, c, 1.0f);
	gfx.DrawTestTriangle();
	gfx.EndFrame();
}