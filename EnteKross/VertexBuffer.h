#pragma once

#include <vector>

#include "Bindable.h"
#include "GraphicsThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<typename Vertex>
	VertexBuffer(Graphics& gfx, std::vector<Vertex> vertices)
		:
		stride((UINT)sizeof(Vertex))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = std::size(vertices) * sizeof(Vertex);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		ENTE_GFX_CHECK_EXCEPTION(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	virtual void Bind(Graphics& gfx) const override
	{
		const UINT offset = 0u;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
private:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};