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

#include <cstdlib>
#include <ctime>

#include "DlgAbout.h"
#include "Version.h"



//----------------------------------------------------------------------------//
// Constructors                                                      DlgAbout //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

DlgAbout::DlgAbout (QWidget* parent) : QMainWindow
	(parent, Qt::Dialog  | Qt::FramelessWindowHint)
{
	// Seed randomizer
	srand (time (NULL));

	// Initialize view
	ui.setupUi (this);

	// Set the version value
	ui.LabelVersion->setText (QString ("2.0.%1.%2").arg (BUILD).arg (REVISION));
}



//----------------------------------------------------------------------------//
// Events                                                            DlgAbout //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void DlgAbout::showEvent (QShowEvent* event)
{
	// Move this window to the center of the parent window
	move (parentWidget()->window()->frameGeometry().topLeft() +
		  parentWidget()->window()->rect().center() - rect().center());

	// Pick random header
	switch (rand() % 3)
	{
		case 0: ui.LabelHeader->setPixmap (QPixmap (":/Images/Header1.png")); break;
		case 1: ui.LabelHeader->setPixmap (QPixmap (":/Images/Header2.png")); break;
		case 2: ui.LabelHeader->setPixmap (QPixmap (":/Images/Header3.png")); break;
	}

	// Start the focus timer
	mFocus = startTimer (40);

	// Call the default base event
	QMainWindow::showEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void DlgAbout::closeEvent (QCloseEvent* event)
{
	// Call the default base event
	QMainWindow::closeEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void DlgAbout::timerEvent (QTimerEvent* event)
{
	// Close on focus loss
	if (!isActiveWindow())
	{
		killTimer (mFocus);
		mFocus = 0; close();
	}
}
