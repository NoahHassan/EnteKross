#pragma once

#include "Graphics.h"
#include "DrawableBase.h"

class Sphere : public DrawableBase<Sphere>
{
public:
	Sphere(Graphics& gfx, float x, float y, float z);
	Sphere(Graphics& gfx);
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SetScale(float scale) noexcept;
public:
	struct
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} position;
	float scale = 1.0f;
};