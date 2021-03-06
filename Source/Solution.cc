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

#include "Solution.h"



//----------------------------------------------------------------------------//
// Constructors                                                      Solution //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

Solution::Solution (void)
{
	Sx = Sy = 0;
	Tx = Ty = 0;
	Matches = 0;
}

////////////////////////////////////////////////////////////////////////////////

Solution::Solution (quint8 sx, quint8 sy,
	quint8 tx, quint8 ty, quint8 matches)
{
	Sx = sx; Sy = sy;
	Tx = tx; Ty = ty;
	Matches = matches;
}



//----------------------------------------------------------------------------//
// Functions                                                         Solution //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/// <summary> Determines whether the coordinates are horizontal. </summary>

bool Solution::IsHorizontal (void) const
{
	return Sx != Tx && Sy == Ty;
}

////////////////////////////////////////////////////////////////////////////////
/// <summary> Determines whether the coordinates are vertical. </summary>

bool Solution::IsVertical (void) const
{
	return Sx == Tx && Sy != Ty;
}



//----------------------------------------------------------------------------//
// Operators                                                         Solution //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

bool Solution::operator == (const Solution& solution) const
{
	return Sx == solution.Sx && Sy == solution.Sy &&
		   Tx == solution.Tx && Ty == solution.Ty &&
		   Matches == solution.Matches;
}

////////////////////////////////////////////////////////////////////////////////

bool Solution::operator != (const Solution& solution) const
{
	return Sx != solution.Sx || Sy != solution.Sy ||
		   Tx != solution.Tx || Ty != solution.Ty ||
		   Matches != solution.Matches;
}
