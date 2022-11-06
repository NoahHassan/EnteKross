#pragma once

#include "Graphics.h"
#include "Drawable.h"

class Boundary : public Drawable
{
public:
	Boundary(Graphics& gfx, float x, float y, float z);
	Boundary(Graphics& gfx);
public:
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;
public:
	struct
	{
		float posX = 0.0f;
		float posY = 0.0f;
		float posZ = 0.0f;
	} position;
};