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

#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "ui_DlgAbout.h"
#include <QMainWindow>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single about dialog window. </summary>

class DlgAbout : public QMainWindow
{
	Q_OBJECT;

public:
	// Constructors
	DlgAbout						(QWidget* parent = NULL);

protected:
	// Events
	virtual void	showEvent		(QShowEvent*  event);
	virtual void	closeEvent		(QCloseEvent* event);
	virtual void	timerEvent		(QTimerEvent* event);

private:
	// Fields
	Ui::DlgAbout	ui;				// Designer interface
	qint32			mFocus;			// Window focus timer
};

#endif // DLG_ABOUT_H
