#include "Cube.h"

#include "Geometry.h"
#include "BindableBase.h"

Cube::Cube(Graphics& gfx, float x, float y, float z)
	:
	position({ x,y,z })
{
	Geometry g = Geometry::MakeCube();
	g.ApplyScale(0.5f);

	AddBind(std::make_unique<VertexBuffer>(gfx, g.vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, g.indices));
	AddBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

	auto pVs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
	auto pByteCode = pVs->GetShaderByteCode();
	AddBind(std::move(pVs));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	AddBind(std::make_unique<InputLayout>(gfx, layout, pByteCode));
	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
}

Cube::Cube(Graphics& gfx)
	:
	Cube(gfx, 0.0f,0.0f,0.0f)
{}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(position.posX, position.posY, position.posZ);
}