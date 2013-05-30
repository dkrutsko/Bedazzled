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

#ifndef WND_OVERLAY_H
#define WND_OVERLAY_H

class Bedazzled;
class Board;
class QGraphicsScene;
class QGraphicsView;

#include <QMainWindow>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single window which overlays the game. </summary>

class WndOverlay : public QMainWindow
{
	Q_OBJECT;

public:
	// Constructors
	WndOverlay								(Board* board, Bedazzled* b,
											 QWidget* parent = NULL);

protected:
	// Events
	virtual void			showEvent		(QShowEvent*  event);
	virtual void			closeEvent		(QCloseEvent* event);
	virtual void			timerEvent		(QTimerEvent* event);

private:
	// Fields
	Board*					mBoard;			// The game board
	Bedazzled*				mBedazzled;		// The Controller
	
	static const QBrush		mSoln1;			// Solution 1 color
	static const QBrush		mSoln2;			// Solution 2 color
	static const QBrush		mSoln3;			// Solution 3 color

	QGraphicsScene*			vScene;			// Graphics scene for the tiles
	QGraphicsView*			vView;			// Graphics view  for the tiles
};

#endif // WND_OVERLAY_H
