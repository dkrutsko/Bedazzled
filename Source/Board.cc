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

#include "Board.h"



//----------------------------------------------------------------------------//
// Constructors                                                         Board //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Board::Board (quint8 w, quint8 h)
{
	// Set size
	mWidth  = w;
	mHeight = h;

	// Create a new tiles array
	Tiles = new Tile* [mWidth];
	for (quint8 x = 0; x < mWidth; ++x)
	{
		Tiles[x] = new Tile [mHeight];
		for (quint8 y = 0; y < mHeight; ++y)
			Tiles[x][y] = Board::None;
	}
}

////////////////////////////////////////////////////////////////////////////////

Board::~Board (void)
{
	// Completely destroy the tiles array
	for (quint8 x = 0; x < mWidth; ++x)
		delete[] Tiles[x]; delete[] Tiles;
}



//----------------------------------------------------------------------------//
// Functions                                                            Board //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Clears this game board using the specified tile. </summary>

void Board::Clear (Tile tile)
{
	for (quint8 x = 0; x < mWidth ; ++x)
	for (quint8 y = 0; y < mHeight; ++y)
		Tiles[x][y] = tile;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the current width of this game board. </summary>

quint8 Board::GetWidth (void) const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns the current height of this game board. </summary>

quint8 Board::GetHeight (void) const
{
	return mHeight;
}
