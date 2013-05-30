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

#ifndef BOARD_H
#define BOARD_H

#include <QMutex>
#include <QList>
#include <QRect>
#include "Solution.h"



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single instance of the game board. </summary>

class Board : public QMutex
{
public:
	////////////////////////////////////////////////////////////////////////////////
	/// <summary> An enumeration of all the possible game tile pieces. </summary>

	enum Tile { None, Black, Blue, Gold, Green, Purple, Red };

public:
	// Constructors
	 Board						(quint8 w, quint8 h);
	~Board						(void);

private:
	// Hidden
	Board						(const Board& board) { }
	Board&			operator =	(const Board& board) { }

public:
	// Functions
	void			Clear		(Tile tile = None);

	quint8			GetWidth	(void) const;
	quint8			GetHeight	(void) const;

public:
	// Properties
	Tile**			Tiles;		// 2D array of tiles [x][y]
	QRect			Grid;		// Game grid area rectangle
	QList<Solution>	Solutions;	// List of current solutions

private:
	// Fields
	quint8			mWidth;		// Width  of the game board
	quint8			mHeight;	// Height of the game board
};

#endif // BOARD_H
