#pragma once

#include <memory>
#include "Graphics.h"

class Bindable
{
public:
	virtual ~Bindable() = default;
public:
	virtual void Bind(Graphics& gfx) const = 0;
protected:
	static ID3D11Device* GetDevice(Graphics& gfx) { return gfx.pDevice.Get(); };
	static ID3D11DeviceContext* GetContext(Graphics& gfx) { return gfx.pContext.Get(); }
};