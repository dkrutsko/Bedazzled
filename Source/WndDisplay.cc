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

#include "DlgAbout.h"
#include "WndDisplay.h"
#include "Board.h"
#include "Bedazzled.h"

#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>



//----------------------------------------------------------------------------//
// Fields                                                          WndDisplay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> A unique windows message for activating the display window. </summary>

const quint32 WndDisplay::mWindowMsg = RegisterWindowMessage (L"ACTIVATE_BEDAZZLED");

////////////////////////////////////////////////////////////////////////////////

const quint32 WndDisplay::mMenuTopMost = 1000;
const quint32 WndDisplay::mMenuAnalyze = 1010;
const quint32 WndDisplay::mMenuOverlay = 1020;
const quint32 WndDisplay::mMenuAbout   = 1030;

////////////////////////////////////////////////////////////////////////////////

const QBrush WndDisplay::mSoln1 = QColor ( 40, 160, 20);
const QBrush WndDisplay::mSoln2 = QColor (160, 160, 20);
const QBrush WndDisplay::mSoln3 = QColor (180,  20, 20);

////////////////////////////////////////////////////////////////////////////////

const QBrush WndDisplay::mLight = QColor (50, 45, 40);
const QBrush WndDisplay::mDark  = QColor (30, 25, 25);



//----------------------------------------------------------------------------//
// Constructors                                                    WndDisplay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

WndDisplay::WndDisplay (Board* board, Bedazzled* b) : QMainWindow (NULL,  0)
{
	// Set members
	mBoard = board;
	mBedazzled = b;

	SetupView();
	SetupMenu();

	mTopMost = false;
	ToggleTopMost();

	// Disable this window's maximize button, necessary due to bug?
	setWindowFlags (windowFlags() & ~Qt::WindowMaximizeButtonHint);

	// Create a new about dialog
	vAbout = new DlgAbout (this);

	// Start window update timer
	startTimer (40);
}



//----------------------------------------------------------------------------//
// Functions                                                       WndDisplay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the windows message for activating this window. </summary>

quint32 WndDisplay::GetWindowMsg (void)
{
	return mWindowMsg;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Creates and initializes all widgets in this window. </summary>

void WndDisplay::SetupView (void)
{
	// Resize window dimensions
	setMinimumSize (496, 496);
	setMaximumSize (496, 496);
	setWindowIcon (QIcon (":/Images/About2.png"));

	// Setup the central widget and layout
	QWidget* widget = new QWidget (this);
	QGridLayout* layout = new QGridLayout (widget);
	layout->setContentsMargins (0, 0, 0, 0);

	// Setup the graphics scene and view
	vScene = new QGraphicsScene (rect());
	vScene->setBackgroundBrush (Qt::black);
	vView = new QGraphicsView (vScene, widget);
	vView->setStyleSheet ("border: none;");

	// Add the graphics view and widget
	layout->addWidget (vView, 0, 0, 1, 1);
	setCentralWidget (widget);

	// Create and initialize all gem image resources
	mGemBlack  = QPixmap (":/Images/GemBlack.png" );
	mGemBlue   = QPixmap (":/Images/GemBlue.png"  );
	mGemGold   = QPixmap (":/Images/GemGold.png"  );
	mGemGreen  = QPixmap (":/Images/GemGreen.png" );
	mGemPurple = QPixmap (":/Images/GemPurple.png");
	mGemRed    = QPixmap (":/Images/GemRed.png"   );
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Creates and initializes the system menu in this window. </summary>

void WndDisplay::SetupMenu (void)
{
	// Retrieve the system menu pointer of this window
	HMENU menu = GetSystemMenu ((HWND) winId(), false);

	// Create shortcuts for common insert flags
	quint32 string = MF_BYPOSITION | MF_STRING;
	quint32 check  = MF_BYPOSITION | MF_STRING | MF_CHECKED;

	// Add additional menu items to this window by index position
	InsertMenu (menu,  5, MF_BYPOSITION | MF_SEPARATOR, 0, L"");
	InsertMenu (menu,  3, check, mMenuTopMost, L"Top Most");
	InsertMenu (menu,  7, check, mMenuAnalyze, L"Enable Analysis");
	InsertMenu (menu,  8, check, mMenuOverlay, L"Display Overlay");
	InsertMenu (menu, 10, string, mMenuAbout, L"About...");

	// Save the menu pointer
	mSysMenu = (void*) menu;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Toggles the top most state of this window. </summary>

void WndDisplay::ToggleTopMost (void)
{
	SetWindowPos ((HWND) winId(),
		(mTopMost = !mTopMost) ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	if (mTopMost)
		ModifyMenu ((HMENU) mSysMenu, 3, MF_BYPOSITION |
		MF_STRING | MF_CHECKED, mMenuTopMost, L"Top Most");
	else
		ModifyMenu ((HMENU) mSysMenu, 3, MF_BYPOSITION |
		MF_STRING | MF_UNCHECKED, mMenuTopMost, L"Top Most");
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Toggles whether or not to enable game analysis. </summary>

void WndDisplay::ToggleAnalyze (void)
{
	if (mBedazzled->IsAnalyzeEnabled())
	{
		mBedazzled->SetAnalyzeEnabled (false);
		ModifyMenu ((HMENU) mSysMenu, 7,
			MF_BYPOSITION | MF_STRING | MF_UNCHECKED,
			mMenuAnalyze, L"Enable Analysis");
	}

	else
	{
		mBedazzled->SetAnalyzeEnabled (true);
		ModifyMenu ((HMENU) mSysMenu, 7,
			MF_BYPOSITION | MF_STRING | MF_CHECKED,
			mMenuAnalyze, L"Enable Analysis");
	}
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Toggles whether to display the overlay window. </summary>

void WndDisplay::ToggleOverlay (void)
{
	if (mBedazzled->IsOverlayEnabled())
	{
		mBedazzled->SetOverlayEnabled (false);
		ModifyMenu ((HMENU) mSysMenu, 8,
			MF_BYPOSITION | MF_STRING | MF_UNCHECKED,
			mMenuOverlay, L"Display Overlay");
	}

	else
	{
		mBedazzled->SetOverlayEnabled (true);
		ModifyMenu ((HMENU) mSysMenu, 8,
			MF_BYPOSITION | MF_STRING | MF_CHECKED,
			mMenuOverlay, L"Display Overlay");
	}
}

////////////////////////////////////////////////////////////////////////////////

bool WndDisplay::nativeEvent
	(const QByteArray& eventType,
	 void* message, long* result)
{
	// Retrieve the event msg
	MSG* msg = (MSG*) message;

	// A system menu item was selected
	if (msg->message == WM_SYSCOMMAND)
		switch ((quint32) msg->wParam)
		{
			case mMenuTopMost:
				ToggleTopMost();
				*result = 0;
				return true;

			case mMenuAnalyze:
				ToggleAnalyze();
				*result = 0;
				return true;

			case mMenuOverlay:
				ToggleOverlay();
				*result = 0;
				return true;

			case mMenuAbout:
				vAbout->show();
				*result = 0;
				return true;
		}

	// Focus this window when multiple instances
	// of this application are attempting to run
	else if (msg->message == WndDisplay::mWindowMsg)
	{
		show();
		activateWindow();
		if (isMinimized())
			showNormal();

		*result = 0;
		return true;
	}

	// Enable window movement from anywhere
	else if (msg->message == WM_NCHITTEST)
	{
		POINT mouse = { LOWORD (msg->lParam),
						HIWORD (msg->lParam) };

		ScreenToClient (msg->hwnd, &mouse);
		if (rect().contains (mouse.x, mouse.y))
			{ *result = HTCAPTION; return true; }
	}

	return false;
}



//----------------------------------------------------------------------------//
// Events                                                          WndDisplay //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void WndDisplay::showEvent (QShowEvent* event)
{
	// Override the Qt window title
	SetWindowText ((HWND) winId(), L"Bedazzled");

	// Call the default base event
	QMainWindow::showEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void WndDisplay::closeEvent (QCloseEvent* event)
{
	// Quit app event loop
	QApplication::quit();

	// Call the default base event
	QMainWindow::closeEvent (event);
}

////////////////////////////////////////////////////////////////////////////////

void WndDisplay::timerEvent (QTimerEvent* event)
{
	//----------------------------------------------------------------------------//
	// Background                                                                 //
	//----------------------------------------------------------------------------//

	// Setup contstant values
	static const quint8 w = 60;
	static const quint8 h = 60;
	static const QPen p (Qt::transparent);

	// Clear scene
	vScene->clear();
	bool alternate = false;

	// Create and initialize all background tiles
	for (quint8 x = 0; x < mBoard->GetWidth(); ++x)
	{
		for (quint8 y = 0; y < mBoard->GetHeight(); ++y)
		{
			vScene->addRect (x * w + 8, y * h + 8, w, h, p,
				(alternate = !alternate) ? mLight : mDark);
		}

		alternate = !alternate;
	}



	//----------------------------------------------------------------------------//
	// Solutions                                                                  //
	//----------------------------------------------------------------------------//

	// Attempt to lock the game board
	if (!mBoard->tryLock (20)) return;

	foreach (const Solution& solution, mBoard->Solutions)
	{
		if (solution.Matches > 2)
		{
			vScene->addRect (solution.Sx * w + 8, solution.Sy * h + 8, w, h, p, mSoln3);
			vScene->addRect (solution.Tx * w + 8, solution.Ty * h + 8, w, h, p, mSoln3);
		}

		else if (solution.Matches > 1)
		{
			vScene->addRect (solution.Sx * w + 8, solution.Sy * h + 8, w, h, p, mSoln2);
			vScene->addRect (solution.Tx * w + 8, solution.Ty * h + 8, w, h, p, mSoln2);
		}

		else
		{
			vScene->addRect (solution.Sx * w + 8, solution.Sy * h + 8, w, h, p, mSoln1);
			vScene->addRect (solution.Tx * w + 8, solution.Ty * h + 8, w, h, p, mSoln1);
		}
	}



	//----------------------------------------------------------------------------//
	// Tiles                                                                      //
	//----------------------------------------------------------------------------//

	for (quint8 x = 0; x < mBoard->GetWidth (); ++x)
	for (quint8 y = 0; y < mBoard->GetHeight(); ++y)
	{
		// Set the tile gem image
		switch (mBoard->Tiles[x][y])
		{
			case Board::Black  : vScene->addPixmap (mGemBlack )->setPos (x * w + 13, y * h + 13); break;
			case Board::Blue   : vScene->addPixmap (mGemBlue  )->setPos (x * w + 13, y * h + 13); break;
			case Board::Gold   : vScene->addPixmap (mGemGold  )->setPos (x * w + 13, y * h + 13); break;
			case Board::Green  : vScene->addPixmap (mGemGreen )->setPos (x * w + 13, y * h + 13); break;
			case Board::Purple : vScene->addPixmap (mGemPurple)->setPos (x * w + 13, y * h + 13); break;
			case Board::Red    : vScene->addPixmap (mGemRed   )->setPos (x * w + 13, y * h + 13); break;
		}
	}

	// Unlock board
	mBoard->unlock();

	// Update view
	vView->update();
}
