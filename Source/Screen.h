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

#ifndef SCREEN_H
#define SCREEN_H

class Window;

#include <QColor>
#include <QRect>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> A helper class for capturing screenshots. </summary>

class Screen
{
public:
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> Represents a single captured screenshot. </summary>

	class Image
	{
		friend class Screen;

	public:
		// Constructors
		 Image						(void);
		~Image						(void);

	private:
		// Hidden
		Image						(const Image& image) { }
		Image&		operator =		(const Image& image) { }

	public:
		// Functions
		quint16		GetWidth		(void) const;
		quint16		GetHeight		(void) const;

		quint32		GetPixel		(quint16 x, quint16 y) const;

	private:
		// Fields
		quint16		mWidth;			// Bitmap width
		quint16		mHeight;		// Bitmap height
		quint32*	mData;			// Pixel data
		quint32		mDataLength;	// Pixel data length
	};

private:
	// Constructors
	 Screen							(void) { }
	~Screen							(void) { }
	 Screen							(const Screen& screen) { }
	 Screen&		operator =		(const Screen& screen) { }

public:
	// Functions
	static QColor	GetPixel		(Window window, const QPoint& point);
	static QColor	GetPixel		(Window window, qint16 x,  qint16 y);

	static void		GetScreen		(Window window, Image* image,
									 const QRect& rect);

	static void		GetScreen		(Window window, Image* image,
									 qint32 x =  0, qint32 y =  0,
									 qint32 w = -1, qint32 h = -1);
};

#endif // SCREEN_H
