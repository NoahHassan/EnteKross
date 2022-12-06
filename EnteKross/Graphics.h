#pragma once

#include "WindowsKross.h"
#include "KrossException.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class Graphics
{
	friend class Bindable;
public:
	class HrException : public KrossException
	{
	public:
		HrException(int line, std::string file, HRESULT hr);
	public:
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		virtual const char* GetType() const noexcept override;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator =(const Graphics&) = delete;
	~Graphics();
public:
	void BeginFrame(float r, float g, float b) noexcept;
	void EndFrame();
	void DrawIndexed(UINT count) const noexcept;
	void SetCameraXM(DirectX::XMMATRIX mat) noexcept;
	DirectX::XMMATRIX GetCameraXM() const noexcept;
	DirectX::XMMATRIX GetProjectionXM() const noexcept;
public:
	void EnableImGui() noexcept;
	void DisableImGui() noexcept;
	bool IsImGuiEnabled() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDSView;

	DirectX::XMMATRIX cameraMat;
	bool imGuiEnabled = true;
};