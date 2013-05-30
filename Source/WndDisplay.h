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

#ifndef WND_DISPLAY_H
#define WND_DISPLAY_H

class DlgAbout;
class Bedazzled;
class Board;
class QGraphicsScene;
class QGraphicsView;

#include <QMainWindow>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single main display window. </summary>

class WndDisplay : public QMainWindow
{
	Q_OBJECT;

public:
	// Constructors
	WndDisplay								(Board* board, Bedazzled* b);

public:
	// Functions
	static quint32			GetWindowMsg	(void);

private:
	void					SetupView		(void);
	void					SetupMenu		(void);

	void					ToggleTopMost	(void);
	void					ToggleAnalyze	(void);
	void					ToggleOverlay	(void);

	virtual bool			nativeEvent		(const QByteArray& eventType,
											 void* message, long* result);

protected:
	// Events
	virtual void			showEvent		(QShowEvent*  event);
	virtual void			closeEvent		(QCloseEvent* event);
	virtual void			timerEvent		(QTimerEvent* event);

private:
	// Fields
	DlgAbout*				vAbout;			// About dialog
	Board*					mBoard;			// The game board
	Bedazzled*				mBedazzled;		// The Controller

	bool					mTopMost;		// Whether window is top most
	void*					mSysMenu;		// Window system menu pointer

	QGraphicsScene*			vScene;			// Graphics scene for the tiles
	QGraphicsView*			vView;			// Graphics view  for the tiles

	QPixmap					mGemBlack;		// Black  gem image resource
	QPixmap					mGemBlue;		// Blue   gem image resource
	QPixmap					mGemGold;		// Gold   gem image resource
	QPixmap					mGemGreen;		// Green  gem image resource
	QPixmap					mGemPurple;		// Purple gem image resource
	QPixmap					mGemRed;		// Red    gem image resource

	static const QBrush		mSoln1;			// Solution 1 color
	static const QBrush		mSoln2;			// Solution 2 color
	static const QBrush		mSoln3;			// Solution 3 color

	static const QBrush		mLight;			// Light tile color
	static const QBrush		mDark;			// Dark  tile color

	static const quint32	mWindowMsg;		// Custom window msg

	static const quint32	mMenuTopMost;	// TopMost menu identifier
	static const quint32	mMenuAnalyze;	// Analyze menu identifier
	static const quint32	mMenuOverlay;	// Overlay menu identifier
	static const quint32	mMenuAbout;		// About   menu identifier
};

#endif // WND_DISPLAY_H
