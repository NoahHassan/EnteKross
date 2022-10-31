#pragma once

#include <vector>

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, std::vector<unsigned short>& indices)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC ibd = {};
		ibd.ByteWidth = sizeof(unsigned short) * (UINT)std::size(indices);
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();

		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}
public:
	void Bind(Graphics& gfx) const override
	{
		GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};