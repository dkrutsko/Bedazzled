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

#include "WndOverlay.h"
#include "Board.h"
#include "Bedazzled.h"
#include "Window.h"

#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>



//----------------------------------------------------------------------------//
// Fields                                                          WndOverlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

const QBrush WndOverlay::mSoln1 = QColor (  0, 255, 0);
const QBrush WndOverlay::mSoln2 = QColor (255, 255, 0);
const QBrush WndOverlay::mSoln3 = QColor (255,   0, 0);



//----------------------------------------------------------------------------//
// Constructors                                                    WndOverlay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

WndOverlay::WndOverlay (Board* board, Bedazzled* b, QWidget* parent)
	: QMainWindow (parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
	// Set members
	mBoard = board;
	mBedazzled = b;

	// Make the window entirely transparent
	setAttribute (Qt::WA_TranslucentBackground);
	setAttribute (Qt::WA_NoSystemBackground   );
	setAttribute (Qt::WA_TransparentForMouseEvents);

	// Setup the central widget and layout
	QWidget* widget = new QWidget (this);
	QGridLayout* layout = new QGridLayout (widget);
	layout->setContentsMargins (0, 0, 0, 0);

	// Setup the graphics scene and view
	vScene = new QGraphicsScene (rect());
	vView = new QGraphicsView (vScene, widget);
	vView->setStyleSheet ("border: none; background: transparent;");

	// Add the graphics view and widget
	layout->addWidget (vView, 0, 0, 1, 1);
	setCentralWidget (widget);

	// Start window update timer
	startTimer (40);
}



//----------------------------------------------------------------------------//
// Events                                                          WndDisplay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void WndOverlay::showEvent (QShowEvent* event)
{
	// Override the Qt window title
	SetWindowText ((HWND) winId(), L"");

	// Call the default base event
	QMainWindow::showEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void WndOverlay::closeEvent (QCloseEvent* event)
{
	// Call the default base event
	QMainWindow::closeEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void WndOverlay::timerEvent (QTimerEvent* event)
{
	// Hide window is game is inactive
	if (Window::GetActive().GetTitle() !=
		"StarCraft II") { hide(); return; }

	// Hide window if overlay is hidden
	if (mBedazzled->IsOverlayEnabled())
		show(); else { hide(); return; }

	// Position and resize the overlay
	setGeometry (mBoard->Grid.x(), mBoard->Grid.y(),
		mBoard->Grid.width(), mBoard->Grid.height());
	vScene->setSceneRect (rect());

	// Clear scene
	vScene->clear();

	quint16 w = (quint16) (width () / mBoard->GetWidth ());
	quint16 h = (quint16) (height() / mBoard->GetHeight());

	quint16 halfW = (quint16) (w / 2);
	quint16 halfH = (quint16) (h / 2);

	// Attempt to lock the game board
	if (!mBoard->tryLock (20)) return;

	foreach (const Solution& solution, mBoard->Solutions)
	{
		QPen pen; pen.setWidth (4);
		pen.setCapStyle (Qt::RoundCap);

			 if (solution.Matches > 2) pen.setBrush (mSoln3);
		else if (solution.Matches > 1) pen.setBrush (mSoln2);
		else pen.setBrush (mSoln1);

		if (solution.IsHorizontal())
		{
			qreal x1 = w * (solution.Sx + 0) + halfW;
			qreal x2 = w * (solution.Tx + 1) - halfW;
			qreal y1 = h * (solution.Sy + 1) - 4;
			qreal y2 = h * (solution.Ty + 1) - 4;

			vScene->addLine (x1, y1, x2, y2, pen);
		}

		else
		{
			qreal x1 = w * (solution.Sx + 1) - 4;
			qreal x2 = w * (solution.Tx + 1) - 4;
			qreal y1 = h * (solution.Sy + 0) + halfH;
			qreal y2 = h * (solution.Ty + 1) - halfH;

			vScene->addLine (x1, y1, x2, y2, pen);
		}
	}

	// Unlock board
	mBoard->unlock();

	// Update view
	vView->update();
}
