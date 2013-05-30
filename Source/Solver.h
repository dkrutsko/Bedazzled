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

#ifndef SOLVER_H
#define SOLVER_H

class Bedazzled;
class Board;

#include <QObject>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> A class for determining all possible game moves. </summary>

class Solver : public QObject
{
	Q_OBJECT;

public:
	// Constructors
	Solver							(Board* board, Bedazzled* b);

public slots:
	// Slots
	void		Update				(void);

private:
	// Functions
	quint8		CheckHorizontal		(const Board* board, quint8 y);
	quint8		CheckVertical		(const Board* board, quint8 x);

private:
	// Fields
	Board*		mBoard;				// The game board
	Bedazzled*	mBedazzled;			// The Controller
};

#endif // SOLVER_H
