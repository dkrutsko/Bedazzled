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

#include "Solver.h"
#include "Board.h"
#include "Bedazzled.h"



//----------------------------------------------------------------------------//
// Constructors                                                        Solver //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Solver::Solver (Board* board, Bedazzled* b) : QObject (NULL)
{
	mBoard = board;
	mBedazzled = b;
}



//----------------------------------------------------------------------------//
// Slots                                                               Solver //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Solver::Update (void)
{
	// Do nothing if analysis stopped
	if (!mBedazzled->IsAnalyzeEnabled())
		return;

	// Lock board
	mBoard->lock();

	// Clear current solutions
	mBoard->Solutions.clear();

	// Compute new solutions
	for (quint8 x = 0; x < mBoard->GetWidth (); ++x)
	for (quint8 y = 0; y < mBoard->GetHeight(); ++y)
	{
		Board::Tile c = mBoard->Tiles[x][y];

		// Check for empty
		if (c == Board::None) continue;

		// Horizontal check
		if (x != mBoard->GetWidth() - 1)
		{
			Board::Tile t = mBoard->Tiles[x+1][y];

			mBoard->Tiles[x  ][y] = t;
			mBoard->Tiles[x+1][y] = c;
						
			qint8 matches1 = CheckVertical   (mBoard, x  ) - 2;
			qint8 matches2 = CheckVertical   (mBoard, x+1) - 2;
			qint8 matches3 = CheckHorizontal (mBoard, y  ) - 2;

			mBoard->Tiles[x  ][y] = c;
			mBoard->Tiles[x+1][y] = t;

			quint8 matches = 0;
			if (matches1 > 0) matches += matches1;
			if (matches2 > 0) matches += matches2;
			if (matches3 > 0) matches += matches3;

			if (matches > 0)
				mBoard->Solutions << Solution (x, y, x+1, y, matches);
		}

		// Vertical check
		if (y != mBoard->GetHeight() - 1)
		{
			Board::Tile t = mBoard->Tiles[x][y+1];

			mBoard->Tiles[x][y  ] = t;
			mBoard->Tiles[x][y+1] = c;

			qint8 matches1 = CheckVertical   (mBoard, x  ) - 2;
			qint8 matches2 = CheckHorizontal (mBoard, y  ) - 2;
			qint8 matches3 = CheckHorizontal (mBoard, y+1) - 2;

			mBoard->Tiles[x][y  ] = c;
			mBoard->Tiles[x][y+1] = t;

			quint8 matches = 0;
			if (matches1 > 0) matches += matches1;
			if (matches2 > 0) matches += matches2;
			if (matches3 > 0) matches += matches3;

			if (matches > 0)
				mBoard->Solutions << Solution (x, y, x, y+1, matches);
		}
	}

	// Unlock board
	mBoard->unlock();
}



//----------------------------------------------------------------------------//
// Functions                                                           Solver //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the maximum number of horizontal matches. </summary>

quint8 Solver::CheckHorizontal (const Board* board, quint8 y)
{
	quint8 max   = 1;
	quint8 count = 1;
	Board::Tile last = board->Tiles[0][y];

	for (quint8 x = 1; x < board->GetWidth(); ++x)
	{
		if (last != board->Tiles[x][y])
		{
			last  = board->Tiles[x][y];
			count = 1;
		}

		else if (++count > max)
			max = count;
	}

	return max;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the maximum number of vertical matches. </summary>

quint8 Solver::CheckVertical (const Board* board, quint8 x)
{
	quint8 max   = 1;
	quint8 count = 1;
	Board::Tile last = board->Tiles[x][0];

	for (quint8 y = 1; y < board->GetHeight(); ++y)
	{
		if (last != board->Tiles[x][y])
		{
			last  = board->Tiles[x][y];
			count = 1;
		}

		else if (++count > max)
			max = count;
	}

	return max;
}
