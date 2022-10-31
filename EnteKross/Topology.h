#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
		:
		topology(topology)
	{}
public:
	void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->IASetPrimitiveTopology(topology);
	}
private:
	D3D11_PRIMITIVE_TOPOLOGY topology;
};