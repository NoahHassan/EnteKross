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

	// Init ImGui
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame(float r, float g, float b) noexcept
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

void Graphics::DrawIndexed(UINT count) const noexcept
{
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::SetCameraXM(DirectX::XMMATRIX mat) noexcept
{
	cameraMat = mat;
}

DirectX::XMMATRIX Graphics::GetCameraXM() const noexcept
{
	return cameraMat;
}

DirectX::XMMATRIX Graphics::GetProjectionXM() const noexcept
{
	return DirectX::XMMatrixPerspectiveRH(1.0f, (3.0f / 4.0f), 0.5f, 1000.0f);
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