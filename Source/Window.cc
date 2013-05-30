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

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "Window.h"
#include <Windows.h>



//----------------------------------------------------------------------------//
// Types                                                                      //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Custom data to pass to the window enumeration callback. </summary>

struct EnumWindowsData
{
	quint32		Count;		// Current number of results
	QString		Title;		// Window title to look for
	bool		Find;		// Whether to stop when found
	void*		Result;		// The result found
};



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Windows callback function for enumerating all windows. </summary>

static BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lParam)
{
	// Check window visibility
	if (IsWindowVisible (hwnd))
	{
		// Retrieve parameter information from lParam
		EnumWindowsData* data = reinterpret_cast<EnumWindowsData*> (lParam);

		// Compare the name of the current window
		if (Window (hwnd).GetTitle() == data->Title)
		{
			if (data->Find)
			{
				data->Result = hwnd;
				return FALSE;
			}

			else ++data->Count;
		}
	}

	return TRUE;
}



//----------------------------------------------------------------------------//
// Constructors                                                        Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Window::Window (void* handle)
{
	mHandle = handle;
}



//----------------------------------------------------------------------------//
// Functions                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the window handle this class is representing. </summary>

void* Window::GetHandle (void) const
{
	return mHandle;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets the window handle for this class to represent. </summary>

void Window::SetHandle (void* handle)
{
	mHandle = handle;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Determines whether or not this window has been closed. </summary>

bool Window::HasExited (void) const
{
	// Check the window
	if (mHandle == NULL)
		return false;

	return !IsWindow ((HWND) mHandle);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the caption bar title of this window. </summary>

QString Window::GetTitle (void) const
{
	// Check the window
	if (mHandle == NULL)
		return QString();

	HWND hwnd = (HWND) mHandle; QString result;
	qint32 length = GetWindowTextLength (hwnd) + 1;

	if (length > 1)
	{
		wchar_t* buffer = new wchar_t[length];
		GetWindowText (hwnd, buffer, length);
		result = QString::fromUtf16 (buffer);
		delete[] buffer;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the client position of this window. </summary>

QPoint Window::GetPos (void) const
{
	return GetRect().topLeft();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the client size of this window. </summary>

QSize Window::GetSize (void) const
{
	return GetRect().size();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the client rectangle of this window. </summary>

QRect Window::GetRect (void) const
{
	// Check the window
	if (mHandle == NULL)
		return QRect();

	RECT rect;
	HWND hwnd = (HWND) mHandle;
	GetClientRect (hwnd, &rect);

	POINT point;
	point.x = rect.left;
	point.y = rect.top;

	ClientToScreen (hwnd, &point);
	return QRect (point.x, point.y,
			rect.right  - rect.left,
			rect.bottom - rect.top);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Converts global screen coordinates to client coordinates. </summary>

QPoint Window::MapToWindow (const QPoint& point)
{
	// Check the window
	if (mHandle == NULL)
		return QPoint();

	POINT p;
	p.x = point.x();
	p.y = point.y();

	HWND hwnd = (HWND) mHandle;
	if (ScreenToClient (hwnd, &p))
		return QPoint (p.x, p.y);
	
	return QPoint();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Converts client coordinates to global screen coordinates. </summary>

QPoint Window::MapToGlobal (const QPoint& point)
{
	// Check the window
	if (mHandle == NULL)
		return QPoint();

	POINT p;
	p.x = point.x();
	p.y = point.y();

	HWND hwnd = (HWND) mHandle;
	if (ClientToScreen (hwnd, &p))
		return QPoint (p.x, p.y);
	
	return QPoint();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Converts global screen coordinates to client coordinates. </summary>

QRect Window::MapToWindow (const QRect& rect)
{
	// Check the window
	if (mHandle == NULL)
		return QRect();

	POINT point1 = { rect.left (), rect.top   () };
	POINT point2 = { rect.right(), rect.bottom() };

	if (ScreenToClient ((HWND) mHandle, &point1) &&
		ScreenToClient ((HWND) mHandle, &point2))
	{
		return QRect (point1.x, point1.y,
					  point2.x - point1.x,
					  point2.y - point1.y);
	}

	return QRect();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Converts client coordinates to global screen coordinates. </summary>

QRect Window::MapToGlobal (const QRect& rect)
{
	// Check the window
	if (mHandle == NULL)
		return QRect();

	POINT point1 = { rect.left (), rect.top   () };
	POINT point2 = { rect.right(), rect.bottom() };

	if (ClientToScreen ((HWND) mHandle, &point1) &&
		ClientToScreen ((HWND) mHandle, &point2))
	{
		return QRect (point1.x, point1.y,
					  point2.x - point1.x,
					  point2.y - point1.y);
	}

	return QRect();
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Finds the first instance of a window matching the title. </summary>

Window Window::Find (const QString& title)
{
	// Prepare data
	EnumWindowsData data;
	data.Count  = 0;
	data.Title  = title;
	data.Find   = true;
	data.Result = NULL;

	// Enumerate windows
	EnumWindows (EnumWindowsProc, (LPARAM) &data);
	return data.Result;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Counts the number of windows with the specified title. </summary>

quint32 Window::Count (const QString& title)
{
	// Prepare data
	EnumWindowsData data;
	data.Count  = 0;
	data.Title  = title;
	data.Find   = false;
	data.Result = NULL;

	// Enumerate windows
	EnumWindows (EnumWindowsProc, (LPARAM) &data);
	return data.Count;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the currently active window. </summary>

Window Window::GetActive (void)
{
	// Attempt to get the foreground window multiple times
	// in case the new window is not fully activated yet

	quint8 times = 0;
	HWND handle;

	while (++times < 20)
	{
		handle = GetForegroundWindow();
		if (handle != NULL) return handle;
		Sleep (50);
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets the currently active window. </summary>

void Window::SetActive (const Window& window)
{
	if (window != NULL)
	{
		HWND hwnd = (HWND) window.mHandle;
		LONG styles = GetWindowLong (hwnd, GWL_STYLE);

		// Restore if minimized
		if (styles & WS_MINIMIZE)
			ShowWindowAsync (hwnd, SW_RESTORE);

		// Set the foreground
		SetForegroundWindow (hwnd);
	}
}



//----------------------------------------------------------------------------//
// Operators                                                           Window //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (const Window& window) const
{
	return mHandle == window.mHandle;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (const Window& window) const
{
	return mHandle != window.mHandle;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator == (const void* handle) const
{
	return mHandle == handle;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::operator != (const void* handle) const
{
	return mHandle != handle;
}
