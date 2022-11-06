#include "App.h"
#include "Particle.h"
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
	pBoundary = std::make_unique<Boundary>(gfx, 2.0f, 0.0f, -5.0f);
	cubes.emplace_back(std::make_unique<Cube>(gfx, 0.0f, 0.0f, -4.0f));
	cubes.emplace_back(std::make_unique<Cube>(gfx, 0.0f, 2.0f, 1.0f));
	cubes.emplace_back(std::make_unique<Cube>(gfx, 1.0f, -3.0f, -6.0f));
}

void App::Update(float dt)
{
	t += dt;
	for (auto& p : cubes)
	{
		p->position.posZ -= dt;
	}
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