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

#include "Bedazzled.h"

#include "Board.h"
#include "Analyzer.h"
#include "Solver.h"

#include "WndDisplay.h"
#include "WndOverlay.h"



//----------------------------------------------------------------------------//
// Constructors                                                     Bedazzled //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Bedazzled::Bedazzled (void)
{
	mContinue = true;
	mAnalyze  = true;
	mOverlay  = true;

	mBoard    = new Board      (8, 8);
	mAnalyzer = new Analyzer   (mBoard, this);
	mSolver   = new Solver     (mBoard, this);
	vDisplay  = new WndDisplay (mBoard, this);

	vOverlay = new WndOverlay
		(mBoard, this, vDisplay);

	// Display window
	vDisplay->show();
}

////////////////////////////////////////////////////////////////////////////////

Bedazzled::~Bedazzled (void)
{
	delete vDisplay;
	delete mSolver;
	delete mAnalyzer;
	delete mBoard;
}



//----------------------------------------------------------------------------//
// Events                                                           Bedazzled //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns whether or not game analysis is enabled. </summary>

bool Bedazzled::IsAnalyzeEnabled (void) const
{
	return mAnalyze;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Returns whether the game overlay window is enabled. </summary>

bool Bedazzled::IsOverlayEnabled (void) const
{
	return mOverlay;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> sets whether or not game analysis is enabled. </summary>

void Bedazzled::SetAnalyzeEnabled (bool enable)
{
	mAnalyze = enable;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets whether the game overlay window is enabled. </summary>

void Bedazzled::SetOverlayEnabled (bool enable)
{
	mOverlay = enable;
}



//----------------------------------------------------------------------------//
// Slots                                                            Bedazzled //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Raises the thread exit flag and waits for termination. </summary>

void Bedazzled::Exit (void)
{
	// Stop the thread
	mContinue = false;
	if (!wait (2000))
		terminate();
}



//----------------------------------------------------------------------------//
// Events                                                           Bedazzled //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

void Bedazzled::run (void)
{
	// Loop until termination
	while (mContinue)
	{
		// Update the model
		mAnalyzer->Update();
		mSolver  ->Update();

		// Wait a while
		msleep (40);
	}
}
