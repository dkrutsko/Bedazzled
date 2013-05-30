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

#ifndef ANALYZER_H
#define ANALYZER_H

class Bedazzled;

#include "Board.h"
#include "Screen.h"
#include "Window.h"
#include <QObject>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> A class for finding and analyzing the in-game board. </summary>

class Analyzer : public QObject
{
	Q_OBJECT;

public:
	// Constructors
	Analyzer						(Board* board, Bedazzled* b);

public slots:
	// Slots
	void			Update			(void);

private:
	// Functions
	QRect			FindGrid		(void);
	Board::Tile		Evaluate		(const Screen::Image& image,
									 quint16 x, quint16 y,
									 quint16 w, quint16 h);

private:
	// Fields
	Board*			mBoard;			// The game board
	Bedazzled*		mBedazzled;		// The Controller

	bool			mClear;			// Clear the board
	Window			mGame;			// The Game window
	QRect			mGrid;			// Game board grid

	Screen::Image	mImage;			// Screenshot image
};

#endif // ANALYZER_H
