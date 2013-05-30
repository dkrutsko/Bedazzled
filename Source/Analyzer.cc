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

#include "Analyzer.h"
#include "Bedazzled.h"



//----------------------------------------------------------------------------//
// Constructors                                                      Analyzer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Analyzer::Analyzer (Board* board, Bedazzled* b) : QObject (NULL)
{
	mBoard = board;
	mBedazzled = b;
	mClear = false;
}



//----------------------------------------------------------------------------//
// Slots                                                             Analyzer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Analyzer::Update (void)
{
	// Reset window if analysis stopped
	if (!mBedazzled->IsAnalyzeEnabled())
		mGame.SetHandle (NULL);

	// Check if the game needs to be found
	if (mGame == NULL || mGame.HasExited())
	{
		if (mBedazzled->IsAnalyzeEnabled())
			mGame = Window::Find ("StarCraft II");

		if (mClear)
		{
			mClear = false;
			mBoard->lock();
			mBoard->Clear();
			mBoard->Solutions.clear();
			mBoard->unlock();
		}
	}

	else
	{
		// Update the board
		mClear = true;
		mGrid = FindGrid();
		if (!mGrid.isNull())
		{
			quint16 tileWidth  = (quint16) (mGrid.width () / mBoard->GetWidth ());
			quint16 tileHeight = (quint16) (mGrid.height() / mBoard->GetHeight());

			Screen::GetScreen (mGame, &mImage, mGrid);

			mBoard->lock();

			for (quint8 x = 0; x < mBoard->GetWidth (); ++x)
			for (quint8 y = 0; y < mBoard->GetHeight(); ++y)
			{
				mBoard->Tiles[x][y] = Evaluate
					(mImage, x * tileWidth, y * tileHeight, tileWidth, tileHeight);
			}

			mBoard->Grid = mGame.MapToGlobal (mGrid);
			mBoard->unlock();
		}
	}
}



//----------------------------------------------------------------------------//
// Functions                                                         Analyzer //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Computes the game board rectangle based on client size. </summary>

QRect Analyzer::FindGrid (void)
{
	// Retrieve the client rectangle
	QRect grid = mGame.GetRect();
	if (grid.isNull()) return grid;

	// Compute the grid location
	float clientW = grid.width ();
	float clientH = grid.height();

	float aspectRatio = clientH / clientW;

	float xValue = clientW / 1024.0f;
	float yValue = 0.0f;
	float cValue = 0.0f;
					
	if (aspectRatio > 0.75f)
	{
		yValue = (clientW * 0.75f) / 720.0f;
		cValue = xValue > yValue ? xValue : yValue;
	}

	else
	{
		yValue = clientH / 720.0f;
		cValue = xValue < yValue ? xValue : yValue;
	}

	grid.setLeft   ((qint32) (411 * cValue));
	grid.setTop    ((qint32) ( 68 * cValue));
	grid.setBottom ((qint32) (452 * cValue));
	grid.setRight  ((qint32) ( 27 * cValue));

	grid.setLeft   ((qint32) clientW - grid.left ());
	grid.setRight  ((qint32) clientW - grid.right());

	return grid;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Determines the distance squared between two colors. </summary>

static inline quint32 Distance
	(quint8 r1, quint8 g1, quint8 b1,
	 quint8 r2, quint8 g2, quint8 b2)
{
	float r = r2 - r1;
	float g = g2 - g1;
	float b = b2 - b1;

	return r * r + g * g + b * b;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Determines the tile at the specified position. </summary>

Board::Tile Analyzer::Evaluate (const Screen::Image& image,
	quint16 x, quint16 y, quint16 w, quint16 h)
{
	quint32 r = 0, g = 0;
	quint32 b = 0, c = 0;

	// Analyze only a subset
	x += (quint16) (w * 0.1f);
	y += (quint16) (h * 0.1f);
	w  = (quint16) (w * 0.8f);
	h  = (quint16) (h * 0.8f);

	// Compute the average
	for (quint16 xx = x; xx < x + w; xx += 4)
	for (quint16 yy = y; yy < y + h; yy += 4)
	{
		++c;
		quint32 pixel = image.GetPixel (xx, yy);

		r += (pixel >> 16) & 0xFF;
		g += (pixel >>  8) & 0xFF;
		b += (pixel >>  0) & 0xFF;
	}

	// Determine the average
	r /= c; g /= c; b /= c;

	// Compare dark colors
	if (Distance (r, g, b,  89,  87,  85) < 1400) return Board::Black;
	if (Distance (r, g, b,  77, 168, 205) < 1400) return Board::Blue;
	if (Distance (r, g, b, 135, 139,  61) < 1400) return Board::Gold;
	if (Distance (r, g, b,  86, 166,  48) < 1400) return Board::Green;
	if (Distance (r, g, b,  81,  32,  90) < 1400) return Board::Purple;
	if (Distance (r, g, b, 144,  50,  24) < 1400) return Board::Red;

	// Compare light colors
	if (Distance (r, g, b, 134, 132, 130) < 900) return Board::Black;
	if (Distance (r, g, b, 116, 190, 219) < 900) return Board::Blue;
	if (Distance (r, g, b, 148, 162, 111) < 900) return Board::Gold;
	if (Distance (r, g, b, 124, 183,  98) < 900) return Board::Green;
	if (Distance (r, g, b, 124,  97, 128) < 900) return Board::Purple;
	if (Distance (r, g, b, 172, 108,  91) < 900) return Board::Red;

	// Cannot determine
	return Board::None;
}
