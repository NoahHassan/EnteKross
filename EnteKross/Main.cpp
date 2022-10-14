#include <sstream>

#include "App.h"

int CALLBACK WinMain(
	_In_	 HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_	 LPSTR lpCmdLine,
    _In_	 int nCmndShow
)
{
	try
	{
		App app{};
		app.Setup();

		while (true)
		{
			Sleep(1);

			if (const auto ecode = Window::ProcessMessages())
			{
				return *ecode;
			}

			app.Update(1.0f/60.0f);
			app.Draw();
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