#include "Window.h"

#include <sstream>

int CALLBACK WinMain(
	_In_	 HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_	 LPSTR lpCmdLine,
    _In_	 int nCmndShow
)
{
	int wheel = 0;
	try
	{
		Window window(800, 600, L"Poopie");
		Graphics& gfx = window.Gfx();

		while (true)
		{
			Sleep(1);

			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}
			gfx.BeginFrame(0.0f, 0.0f, 1.0f);
			gfx.EndFrame();
		}
	}
	catch (KrossException& ex)
	{
		MessageBoxA(NULL, ex.what(), ex.GetType(), MB_ICONERROR | MB_OK);
	}
	catch (std::exception& ex)
	{
		MessageBoxA(NULL, ex.what(), "Standard Exception", MB_ICONERROR | MB_OK);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unhandled exception", "Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}