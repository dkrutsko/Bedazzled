////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                        (C) 2011-2013  David Krutsko                        //
//                        See LICENSE.md for copyright                        //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "Bedazzled.h"
#include "WndDisplay.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <QtPlugin>
#include <QApplication>

// Force the windows platform plugin to link
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);

// A unique application name for the mutex
#define APPLICATION_NAME L"Bedazzled-4815"

// Function prototype for disabling aero
extern "C" { typedef HRESULT (WINAPI
	*DWMENABLECOMPOSITION) (UINT action); }



//----------------------------------------------------------------------------//
// Main                                                                       //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Main execution point for this application. </summary>
/// <param name="argc"> Number of arguments in the command line. </param>
/// <param name="argv"> Arguments from the command line. </param>
/// <returns> Zero for success, error code for failure. </returns>

int main (int argc, char** argv)
{
	// Attempt to open mutex
	HANDLE mutex = OpenMutex
		(MUTEX_ALL_ACCESS,
		0, APPLICATION_NAME);

	if (mutex == NULL)
	{
		// First instance of app
		mutex = CreateMutex (NULL,
			TRUE, APPLICATION_NAME);

		// Attempt to access DwmAPI and disable aero
		HMODULE dwmapi = LoadLibrary (L"Dwmapi.dll");
		if (dwmapi != NULL)
		{
			// Disable windows aero
			reinterpret_cast <DWMENABLECOMPOSITION>
				(GetProcAddress (dwmapi,
				"DwmEnableComposition")) (0);

			// Free the DwmAPI DLL
			FreeLibrary (dwmapi);
		}

		// Create main Qt Application
		QApplication app (argc, argv);

		// Start controller
		Bedazzled bedazzled;
		bedazzled.start();

		// Execute application
		int result = QApplication::exec();

		// Stop controller
		bedazzled.Exit();

		// Release app lock
		ReleaseMutex (mutex);
		CloseHandle  (mutex);
		return result;
	}

	else
	{
		// Application already running, broadcast the activate window message
		PostMessage (HWND_BROADCAST, WndDisplay::GetWindowMsg(), NULL, NULL);
	}

	return 0;
}
