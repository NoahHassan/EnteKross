#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Graphics.h"
#include "dxerr.h"
#include "Geometry.h"
#include "GraphicsThrowMacros.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	// For error-checking
	HRESULT hr;

	// Configure Device and Swap Chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	// Create Device and Swap Chain
	UINT swapCreateFlags = 0;
#ifdef _DEBUG
	swapCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	ENTE_GFX_CHECK_EXCEPTION(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		swapCreateFlags,
		NULL, 0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext
	));

	// Create Render Target View
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	ENTE_GFX_CHECK_EXCEPTION(pSwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView));

	// Bind Render Target View
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	// Configure Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	// Create Position Buffer
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	D3D11_BUFFER_DESC cbd = {};
	cbd.ByteWidth			= sizeof(ConstantBuffer);
	cbd.Usage				= D3D11_USAGE_DYNAMIC;
	cbd.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags			= 0u;
	cbd.StructureByteStride = 0u;

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&cbd, nullptr, &pPositionBuffer));

	// Init ImGui
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

Graphics::~Graphics()
{
	ImGui::DestroyContext();
	ImGui_ImplDX11_Shutdown();
}

void Graphics::BindPrimitive(Geometry geometry)
{
	namespace wrl = Microsoft::WRL;
	namespace dx = DirectX;

	HRESULT hr;

	primitiveIndexCount = (UINT)std::size(geometry.indices);
	std::vector<dx::XMFLOAT3> vertices = geometry.vertices;
	std::vector<unsigned short> indices = geometry.indices;

	// Create Vertex Buffer
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = (UINT)std::size(geometry.vertices) * sizeof(dx::XMFLOAT3);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(dx::XMFLOAT3);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// Bind Vertex Buffer
	const UINT stride = sizeof(dx::XMFLOAT3);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create Index Buffer
	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(unsigned short) * (UINT)std::size(geometry.indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = geometry.indices.data();

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	// Bind Index Buffer
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(L"SolidPS.cso", &pBlob));
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// Bind Pixel Shader
	pContext->PSSetShader(pPixelShader.Get(), 0, 0);

	// Create Vertex Shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(L"SolidVS.cso", &pBlob));
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// Bind Vertex Shader
	pContext->VSSetShader(pVertexShader.Get(), 0, 0);

	// Set input (vertex) layout
	wrl::ComPtr<ID3D11InputLayout> pLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout));

	// Bind input layout
	pContext->IASetInputLayout(pLayout.Get());

	// Bind Render Target
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	// Set Primitive Topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::BeginFrame(float r, float g, float b)
{
	if (imGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[4] = { r,g,b,1.0f };
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}

void Graphics::EndFrame()
{
	if (imGuiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == ERROR_DEVICE_REMOVED)
		{
			ENTE_GFX_THROW_DEVICE_REMOVED_EXCEPTION(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			ENTE_GFX_THROW_EXCEPTION(hr);
		}
	}
}

void Graphics::DrawAt(float x, float y, float z)
{
	HRESULT hr;
	DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixTranslation(x, y, z) *
		DirectX::XMMatrixPerspectiveRH(1.0f, (3.0f / 4.0f), 0.5f, 1000.0f)
	);

	D3D11_MAPPED_SUBRESOURCE msr;
	ENTE_GFX_CHECK_EXCEPTION(pContext->Map(
		pPositionBuffer.Get(),
		0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr
	));
	std::memcpy(msr.pData, &transform, sizeof(transform));
	pContext->Unmap(pPositionBuffer.Get(), 0u);

	pContext->VSSetConstantBuffers(0u, 1u, pPositionBuffer.GetAddressOf());

	pContext->DrawIndexed(primitiveIndexCount, 0u, 0u);
}

void Graphics::DrawTestCube(float t)
{
	namespace wrl = Microsoft::WRL;
	namespace dx = DirectX;

	HRESULT hr;

	// Create Vertex Buffer
	struct Vertex
	{
		struct {
			float x;
			float y;
			float z;
		} pos;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} col;
	};

	const Vertex vertices[8] =
	{
		{-1.0f, 1.0f,-1.0f, 255, 255, 255},	// 0
		{ 1.0f, 1.0f,-1.0f, 255, 255,   0},	// 1
		{ 1.0f,-1.0f,-1.0f, 255,   0, 255},	// 2
		{-1.0f,-1.0f,-1.0f, 255,   0,   0},	// 3
		{-1.0f, 1.0f, 1.0f,   0, 255, 255},	// 4
		{ 1.0f, 1.0f, 1.0f,   0, 255,   0},	// 5
		{ 1.0f,-1.0f, 1.0f,   0,   0, 255},	// 6
		{-1.0f,-1.0f, 1.0f,   0,   0,   0}	// 7
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// Bind Vertex Buffer
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create Index Buffer
	const unsigned short indices[] =
	{
		0,2,1, 0,3,2, // Front
		1,6,5, 1,2,6, // Right
		5,7,4, 5,6,7, // Back
		4,3,0, 4,7,3, // Left
		4,1,5, 4,0,1, // Top
		3,6,2, 3,7,6  // Bottom
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	// Bind Index Buffer
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create Constant Buffer
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	D3D11_BUFFER_DESC cbd = {};
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&cbd, nullptr, &pPositionBuffer));

	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationX(t) *
				dx::XMMatrixRotationY(t) *
				dx::XMMatrixRotationZ(t) *
				dx::XMMatrixTranslation(0.0f,0.0f,-4.0f) *
				dx::XMMatrixPerspectiveRH(1.0f, (3.0f / 4.0f), 0.5f,10.0f)
			)
		}
	};

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;

	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	// Bind Constant Buffer
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// Create Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// Bind Pixel Shader
	pContext->PSSetShader(pPixelShader.Get(), 0, 0);

	// Create Vertex Shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	ENTE_GFX_CHECK_EXCEPTION(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// Bind Vertex Shader
	pContext->VSSetShader(pVertexShader.Get(), 0, 0);

	// Set input (vertex) layout
	wrl::ComPtr<ID3D11InputLayout> pLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	ENTE_GFX_CHECK_EXCEPTION(pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout));

	// Bind input layout
	pContext->IASetInputLayout(pLayout.Get());

	// Bind Render Target
	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	// Set Primitive Topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Configure Viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Graphics::EnableImGui() noexcept
{
	imGuiEnabled = true;
}

void Graphics::DisableImGui() noexcept
{
	imGuiEnabled = false;
}

bool Graphics::IsImGuiEnabled() const noexcept
{
	return imGuiEnabled;
}

Graphics::HrException::HrException(int line, std::string file, HRESULT hr)
	:
	KrossException(line, file),
	hr(hr)
{}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream err;
	err << GetType() <<
		"\nLine: " << GetLine() <<
		"\nFile: " << GetFile() <<
		"\nError Code: 0x" << std::hex << std::uppercase << GetErrorCode() <<
		" (" << std::dec << GetErrorCode() << ")" <<
		"\nError String: " << GetErrorString() <<
		"\nDescription: " << GetErrorDescription() << std::endl;

	whatBuffer = err.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "EnteKross Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buffer[512];
	DXGetErrorDescriptionA(hr, buffer, sizeof(buffer));
	return buffer;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "EnteKross Graphics Exception [Device Removed]";
}