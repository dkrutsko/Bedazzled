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

#ifndef SOLUTION_H
#define SOLUTION_H

#include <QGlobal.h>



//----------------------------------------------------------------------------//
// Classes                                                                    //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Represents a single solution on the game board. </summary>

class Solution
{
public:
	// Constructors
	Solution				(void);
	Solution				(quint8 sx, quint8 sy,
							 quint8 tx, quint8 ty,
							 quint8 matches = 1);

public:
	// Functions
	bool	IsHorizontal	(void) const;
	bool	IsVertical		(void) const;

public:
	// Operators
	bool	operator ==		(const Solution& solution) const;
	bool	operator !=		(const Solution& solution) const;

public:
	// Properties
	quint8	Sx;				// Source X position
	quint8	Sy;				// Source Y position
	quint8	Tx;				// Target X position
	quint8	Ty;				// Target Y position

	quint8	Matches;		// Number of matches
};

#endif // SOLUTION_H
