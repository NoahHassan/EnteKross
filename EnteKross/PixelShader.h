#pragma once

#include <d3dcompiler.h>
#include <vector>

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const wchar_t* fileName)
	{
		HRESULT hr;

		ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(fileName, &pBlob));
		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}
public:
	void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), 0, 0);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};