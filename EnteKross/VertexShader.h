#pragma once

#include <d3dcompiler.h>
#include <vector>

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, wchar_t* fileName)
	{
		HRESULT hr;

		ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(fileName, &pBlob));
		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	}
public:
	void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), 0, 0);
	}
	ID3DBlob* GetShaderByteCode() const
	{
		return pBlob.Get();
	}
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};