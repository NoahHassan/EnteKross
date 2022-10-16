#include "App.h"
#include "Particle.h"
#include "Geometry.h"

#include <cmath>
#include <random>

App::App()
	:
	wnd(800, 600, L"Ente Kross"),
	gfx(wnd.Gfx())
{}

void App::Setup()
{
	Geometry primitive = Geometry::MakeIcosphere();
	primitive.ApplyScale(0.1f);
	gfx.BindPrimitive(primitive);

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dst(1.0f, 10.0f);

	Vec3 offset = { 5.0f, 5.0f, 20.0f };
	for (int i = 0; i < 500; i++)
	{
		Vec3 pos = Vec3{ dst(rng), dst(rng), dst(rng) };
		Vec3 vel = Vec3::Dot(pos, { 0.0f,0.0f,1.0f }) * 0.1f;
		particles.emplace_back(1.0f, pos - offset, vel);
	}
}

void App::Update(float dt)
{
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (i != j)
			{
				Vec3 dist = particles[i].GetPos() - particles[j].GetPos();
				float distSqr = dist.GetLengthSqr();

				Vec3 force = dist * (-1.0f / distSqr);
				particles[i].AddForce(force);
				particles[j].AddForce(-force);
			}
		}
	}

	for (auto& p : particles)
	{
		p.Update(dt);
		p.ResetForces();
	}
}

void App::Draw()
{
	gfx.BeginFrame(0.2, 0.2, 0.3);
	for (auto& p : particles)
	{
		p.Draw(gfx);
	}
	gfx.EndFrame();
}