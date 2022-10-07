#pragma once

#include <optional>
#include <memory>

#include "WindowsKross.h"
#include "KrossException.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"

class Window
{
private:
	class Exception : public KrossException
	{
		using KrossException::KrossException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, std::string file, HRESULT hr) noexcept;
	public:
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
		using Exception::Exception;
	public:
		virtual const char* GetType() const noexcept override;
	};
private:
	class WindowClass
	{
	public:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator =(const WindowClass&) = delete;
	public:
		static HINSTANCE GetInstance() noexcept;
		static const wchar_t* GetName() noexcept;
	private:
		static constexpr const wchar_t* pClassName = L"EnteKross Window Class";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(unsigned int width, unsigned int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
public:
	static LRESULT CALLBACK HandleMsgCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	static std::optional<int> ProcessMessages() noexcept;
	void SetTitle(const std::string& title);
	Graphics& Gfx() const noexcept;
public:
	Mouse mouse;
	Keyboard keyboard;
private:
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	int width;
	int height;
	const wchar_t* name;
private:
	bool quitDialog = false;
};

#define ENTE_WND_THROW_EXCEPTION(hr) throw Window::HrException(__LINE__, __FILE__, hr)
#define ENTE_WND_THROW_LAST_EXCEPTION() throw Window::HrException(__LINE__, __FILE__, GetLastError())
#define ENTE_WND_THROW_NOGFX_EXCEPTION() throw Window::NoGfxException(__LINE__, __FILE__)