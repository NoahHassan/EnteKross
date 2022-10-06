#pragma once

#include <optional>

#include "WindowsKross.h"
#include "KrossException.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window
{
private:
	class Exception : public KrossException
	{
	public:
		Exception(int line, std::string file, HRESULT hr) noexcept;
	public:
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string GetErrorDescritpion() const noexcept;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	private:
		HRESULT hr;
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
	static std::optional<int> ProcessMessages() noexcept;
	void SetTitle(const std::string& title);
public:
	Mouse mouse;
	Keyboard keyboard;
private:
	HWND hWnd;
	unsigned int width;
	unsigned int height;
	const wchar_t* name;
private:
	bool quitDialog = false;
};

#define ENTE_WND_THROW_EXCEPTION(hr) throw Window::Exception(__LINE__, __FILE__, hr)
#define ENTE_WND_THROW_LAST_EXCEPTION() throw Window::Exception(__LINE__, __FILE__, GetLastError())