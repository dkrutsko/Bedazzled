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

#ifndef WINDOW_H
#define WINDOW_H

#include <QRect>
#include <QString>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> A helper class for representing a single window. </summary>

class Window
{
public:
	// Constructors
	Window									(void* handle = NULL);

public:
	// Functions
	void*				GetHandle			(void) const;
	void				SetHandle			(void* handle);

	bool				HasExited			(void) const;
	QString				GetTitle			(void) const;

	QPoint				GetPos				(void) const;
	QSize				GetSize				(void) const;
	QRect				GetRect				(void) const;

	QPoint				MapToWindow			(const QPoint& point);
	QPoint				MapToGlobal			(const QPoint& point);

	QRect				MapToWindow			(const QRect& rect);
	QRect				MapToGlobal			(const QRect& rect);

	static Window		Find				(const QString& title);
	static quint32		Count				(const QString& title);

	static Window		GetActive			(void);
	static void			SetActive			(const Window& window);

public:
	// Operators
	bool				operator ==			(const Window& window) const;
	bool				operator !=			(const Window& window) const;

	bool				operator ==			(const void* handle) const;
	bool				operator !=			(const void* handle) const;

private:
	// Fields
	void*				mHandle;			// Handle to the window
};

#endif // WINDOW_H
