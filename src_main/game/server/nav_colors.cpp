//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

// Central point for defining colors and drawing routines for Navigation Mesh edit mode
// Author: Matthew Campbell, 2004

#include "cbase.h"
#include "nav_colors.h"
#include "Color.h"

//--------------------------------------------------------------------------------------------------------------
/**
 * This MUST be kept in sync with the NavEditColor definition
 */
Color NavColors[] =
{
	// Degenerate area colors
	Color( 255, 255, 255 ),		// NavDegenerateFirstColor
	Color( 255, 0, 255 ),		// NavDegenerateSecondColor

	// Place painting color
	Color( 0, 255, 0 ),			// NavSamePlaceColor
	Color( 0, 0, 255 ),			// NavDifferentPlaceColor
	Color( 255, 0, 0 ),			// NavNoPlaceColor

	// Normal colors
	Color( 255, 255, 0 ),		// NavSelectedColor
	Color( 0, 255, 255 ),		// NavMarkedColor
	Color( 255, 0, 0 ),			// NavNormalColor
	Color( 0, 0, 255 ),			// NavCornerColor
	Color( 0, 0, 255 ),			// NavBlockedColor

	// Hiding spot colors
	Color( 255, 0, 0 ),			// NavIdealSniperColor
	Color( 255, 0, 255 ),		// NavGoodSniperColor
	Color( 0, 255, 0 ),			// NavGoodCoverColor
	Color( 255, 0, 255 ),		// NavExposedColor
	Color( 255, 100, 0 ),		// NavApproachPointColor

	// Connector colors
	Color( 0, 255, 255 ),		// NavConnectedTwoWaysColor
	Color( 0, 0, 255 ),			// NavConnectedOneWayColor

	// Editing colors
	Color( 255, 255, 255 ),		// NavCursorColor
	Color( 255, 255, 255 ),		// NavSplitLineColor
	Color( 0, 255, 255 ),		// NavCreationColor
	Color( 0, 64, 64 ),			// NavGridColor

	// Nav attribute colors
	Color( 0, 0, 255 ),			// NavAttributeCrouchColor
	Color( 0, 255, 0 ),			// NavAttributeJumpColor
	Color( 0, 255, 0 ),			// NavAttributePreciseColor
	Color( 255, 0, 0 ),			// NavAttributeNoJumpColor
	Color( 255, 0, 0 ),			// NavAttributeStopColor
	Color( 0, 0, 255 ),			// NavAttributeRunColor
	Color( 0, 255, 0 ),			// NavAttributeWalkColor
	Color( 255, 0, 0 ),			// NavAttributeAvoidColor
};


//--------------------------------------------------------------------------------------------------------------
void NavDrawLine( const Vector& from, const Vector& to, NavEditColor navColor )
{
	const Vector offset( 0, 0, 1 );

	Color color = NavColors[navColor];
	NDebugOverlay::Line( from + offset, to + offset, color[0]/2, color[1]/2, color[2]/2, true, 0.1f );
	NDebugOverlay::Line( from + offset, to + offset, color[0], color[1], color[2], false, 0.15f );
}


//--------------------------------------------------------------------------------------------------------------
void NavDrawTriangle( const Vector& point1, const Vector& point2, const Vector& point3, NavEditColor navColor )
{
	NavDrawLine( point1, point2, navColor );
	NavDrawLine( point2, point3, navColor );
	NavDrawLine( point1, point3, navColor );
}


//--------------------------------------------------------------------------------------------------------------
void NavDrawHorizontalArrow( const Vector& from, const Vector& to, float width, NavEditColor navColor )
{
	const Vector offset( 0, 0, 1 );

	Color color = NavColors[navColor];
	NDebugOverlay::HorzArrow( from + offset, to + offset, width, color[0]/2, color[1]/2, color[2]/2, 255, true, 0.1f );
	NDebugOverlay::HorzArrow( from + offset, to + offset, width, color[0], color[1], color[2], 255, false, 0.15f );
}


//--------------------------------------------------------------------------------------------------------------
void NavDrawDashedLine( const Vector& from, const Vector& to, NavEditColor navColor )
{
	const Vector offset( 0, 0, 1 );

	Color color = NavColors[navColor];

	const float solidLen = 7.0f;
	const float   gapLen = 3.0f;

	Vector unit = (to - from);
	const float totalDistance = unit.NormalizeInPlace();

	float distance = 0.0f;

	while ( distance < totalDistance )
	{
		Vector start = from + unit * distance;
		float endDistance = distance + solidLen;
		endDistance = min( endDistance, totalDistance );
		Vector end = from + unit * endDistance;

		distance += solidLen + gapLen;

		NDebugOverlay::Line( start + offset, end + offset, color[0]/2, color[1]/2, color[2]/2, true, 0.1f );
		NDebugOverlay::Line( start + offset, end + offset, color[0], color[1], color[2], false, 0.15f );
	}
}


//--------------------------------------------------------------------------------------------------------------
