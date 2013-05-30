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

#ifndef BEDAZZLED_H
#define BEDAZZLED_H

class Board;
class Analyzer;
class Solver;

class WndDisplay;
class WndOverlay;

#include <QThread>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Controls and manages all aspects of this application. </summary>

class Bedazzled : public QThread
{
	Q_OBJECT;

public:
	// Constructors
	 Bedazzled							(void);
	~Bedazzled							(void);

public:
	// Functions
	bool			IsAnalyzeEnabled	(void) const;
	bool			IsOverlayEnabled	(void) const;

	void			SetAnalyzeEnabled	(bool enable);
	void			SetOverlayEnabled	(bool enable);

public slots:
	// Slots
	void			Exit				(void);

protected:
	// Events
	void			run					(void);

private:
	// Fields
	volatile bool	mContinue;			// Continue running
	volatile bool	mAnalyze;			// Enable analyze
	volatile bool	mOverlay;			// Enable overlay

	Board*			mBoard;				// The game board

	Analyzer*		mAnalyzer;			// Game analyzer
	Solver*			mSolver;			// Game solver

	WndDisplay*		vDisplay;			// Display window
	WndOverlay*		vOverlay;			// Overlay window
};

#endif // BEDAZZLED_H
