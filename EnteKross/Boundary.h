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
	void SetScale(float sx, float sy, float sz) noexcept;
	void SetColor(float r, float g, float b) noexcept;
private:
	struct RGBColor
	{
		float r;
		float g;
		float b;
		float a;
	};
private:
	RGBColor color = { 1.0f,1.0f,1.0f,1.0f };
	struct
	{
		float x = 1.0f;
		float y = 1.0f;
		float z = 1.0f;
	} scale;
public:
	struct
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} position;
};