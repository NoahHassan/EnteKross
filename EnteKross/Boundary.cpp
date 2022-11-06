#include "Boundary.h"

#include "Geometry.h"
#include "BindableBase.h"

Boundary::Boundary(Graphics& gfx, float x, float y, float z)
	:
	position({ x,y,z })
{
	Geometry g = Geometry::MakeCubeWire();

	AddBind(std::make_unique<VertexBuffer>(gfx, g.vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, g.indices));
	AddBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

	auto pVs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
	auto pByteCode = pVs->GetShaderByteCode();
	AddBind(std::move(pVs));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST));

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, layout, pByteCode));
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

Boundary::Boundary(Graphics& gfx)
	:
	Boundary(gfx, 0.0f, 0.0f, 0.0f)
{}

DirectX::XMMATRIX Boundary::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(position.posX, position.posY, position.posZ);
}