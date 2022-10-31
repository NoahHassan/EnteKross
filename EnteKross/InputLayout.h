#pragma once

#include <d3dcompiler.h>
#include <vector>

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, std::vector<D3D11_INPUT_ELEMENT_DESC> ied, ID3DBlob* pByteCode)
	{
		HRESULT hr;

		ENTE_GFX_CHECK_EXCEPTION(
			GetDevice(gfx)->CreateInputLayout(
				ied.data(), (UINT)std::size(ied),
				pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(),
				&pLayout)
		);
	}
	void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->IASetInputLayout(pLayout.Get());
	}
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
};