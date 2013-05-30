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
#include "Screen.h"
#include "Window.h"
#include <Windows.h>



//----------------------------------------------------------------------------//
// Constructors                                                 Screen::Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Screen::Image::Image (void)
{
	mWidth		= 0;
	mHeight		= 0;
	mData		= NULL;
	mDataLength	= 0;
}

////////////////////////////////////////////////////////////////////////////////

Screen::Image::~Image (void)
{
	if (mData != NULL)
		delete[] mData;
}



//----------------------------------------------------------------------------//
// Functions                                                    Screen::Image //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the current width of this image. </summary>

quint16 Screen::Image::GetWidth (void) const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the current height of this image. </summary>

quint16 Screen::Image::GetHeight (void) const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns a single pixel at the specified coordinates. </summary>

quint32 Screen::Image::GetPixel (quint16 x, quint16 y) const
{
	if (x >= mWidth || y >= mHeight) return 0;
	return mData[x + (y * mWidth)] & 0x00FFFFFF;
}



//----------------------------------------------------------------------------//
// Functions                                                           Screen //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the color of the pixel at the specified position. </summary>
/// <remarks> If the window is null then the whole screen is sampled. </remarks>

QColor Screen::GetPixel (Window window, const QPoint& point)
{
	return GetPixel (window, point.x(), point.y());
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the color of the pixel at the specified position. </summary>
/// <remarks> If the window is null then the whole screen is sampled. </remarks>

QColor Screen::GetPixel (Window window, qint16 x, qint16 y)
{
	HWND hwnd = (HWND) window.GetHandle();

	HDC hSrcDC = GetDC (hwnd);
	COLORREF ref = ::GetPixel (hSrcDC, x, y);
	ReleaseDC (hwnd, hSrcDC);

	return QColor
		((ref >> 16) & 0xFF,
		 (ref >>  8) & 0xFF,
		 (ref >>  0) & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Takes a single screenshot of the specified window. </summary>
/// <param name="window"> The specific window to take a screenshot of. </param>
/// <param name="image"> An image instance to store the results in. </param>
/// <remarks> An single image instance can be reused, regardless of size.
///           If the window is null then the whole screen is sampled. </remarks>

void Screen::GetScreen (Window window, Image* image, const QRect& rect)
{
	GetScreen (window, image, rect.x(), rect.y(), rect.width(), rect.height());
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Takes a single screenshot of the specified window. </summary>
/// <param name="window"> The specific window to take a screenshot of. </param>
/// <param name="image"> An image instance to store the results in. </param>
/// <remarks> An single image instance can be reused, regardless of size.
///           If the window is null then the whole screen is sampled. </remarks>

void Screen::GetScreen (Window window, Image* image,
	qint32 x, qint32 y, qint32 w, qint32 h)
{
	// Ensure that image and dimensions are valid
	if (image == NULL || w == 0 || h == 0) return;

	// Get the size of the window's client
	RECT rect;
	HWND hwnd = (HWND) window.GetHandle();
	GetClientRect (hwnd, &rect);

	if (w < 0) w = rect.right  - rect.left;
	if (h < 0) h = rect.bottom - rect.top;

	// Create a new bitmap
	image->mWidth  = w;
	image->mHeight = h;
	quint32 length = w * h;

	// Check if the bitmap can be reused
	if (image->mDataLength < length)
	{
		if (image->mData != NULL)
			delete[] image->mData;

		image->mData = new quint32[length];
		image->mDataLength = length;
	}

	// Create the source DC
	HDC hSrcDC = GetDC (hwnd);

	// Create compatible destination objects
	HDC hDstDC = CreateCompatibleDC (hSrcDC);
	HBITMAP hImage = CreateCompatibleBitmap (hSrcDC, w, h);
	HGDIOBJ hOldDC = SelectObject (hDstDC, hImage);

	// Copy the specified area in the source DC
	BitBlt (hDstDC, 0, 0, w, h, hSrcDC, x, y, SRCCOPY);

	// Convert the HBITMAP into a Bitmap
	BITMAPINFO bmi;
	memset (&bmi, 0, sizeof (BITMAPINFO));

	bmi.bmiHeader.biSize        = sizeof (BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       =  w;
	bmi.bmiHeader.biHeight      = -h;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;

	HDC display = GetDC (NULL);
	GetDIBits (display, hImage, 0, h, image->mData, &bmi, DIB_RGB_COLORS);
	ReleaseDC (NULL, display);

	// Dispose and clean up objects
	SelectObject (hDstDC, hOldDC);

	DeleteDC (hDstDC);
	ReleaseDC (hwnd, hSrcDC);
	DeleteObject (hImage);
}
