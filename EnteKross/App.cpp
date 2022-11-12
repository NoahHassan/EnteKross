#include "App.h"
#include "Geometry.h"
#include "imgui.h"

#include <cmath>


App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
	pBoundary = std::make_unique<Boundary>(gfx, 0.0f, 0.0f, 0.0f);
	cubes.emplace_back(std::make_unique<Cube>(gfx, 0.0f, 0.0f, -4.0f));
	cubes.emplace_back(std::make_unique<Cube>(gfx, 0.0f, 2.0f, 1.0f));
	cubes.emplace_back(std::make_unique<Cube>(gfx, 1.0f, -3.0f, -6.0f));

	cam.UpdatePosition(gfx, { 0.0f,0.0f,5.0f });
}

void App::Update(float dt)
{
	t += dt;
	for (auto& p : cubes)
	{
		p->position.posZ -= dt;
	}

	// Keyboard Control
	if (wnd.keyboard.KeyIsPressed('D'))
	{
		phi -= camSpeed * dt;
	}
	else if (wnd.keyboard.KeyIsPressed('A'))
	{
		phi += camSpeed * dt;
	}

	if (wnd.keyboard.KeyIsPressed('W') && theta > theta_min)
	{
		theta -= camSpeed * dt;
	}
	else if (wnd.keyboard.KeyIsPressed('S') && theta < theta_max)
	{
		theta += camSpeed * dt;
	}
	// Keyboard Control

	cam.UpdatePosition(gfx, { r * std::sin(theta) * std::cos(phi),r * std::cos(theta),r * std::sin(theta) * std::sin(phi) });
}

void App::Draw()
{
	gfx.BeginFrame(c, c, 1.0f);

	// DRAW CODE HERE //

	if (ImGui::Begin("Chicken Tikka"))
	{
		ImGui::SliderFloat("Background", &c, 0.0f, 1.0f, "%.2f");
	}
	ImGui::End();

	for (auto& p : cubes)
	{
		p->Draw(gfx);
	}
	pBoundary->Draw(gfx);

	// DRAW CODE HERE //

	gfx.EndFrame();
}