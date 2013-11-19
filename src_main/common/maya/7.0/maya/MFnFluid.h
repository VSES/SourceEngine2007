#ifndef _MFnFluid
#define _MFnFluid
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MFnFluid
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnFluid)
//
// This is the function set for fluid objects.
// 
// A fluid object is a node in the dependency graph that contains a grid which 
// is made up of cells.  Each cell has a variety of values assigned to it such
// as density, velocity, temperature, and color.  The grid may be 2D or 3D.  
// See the methods below for full details.
// 
// Fluid objects may be used for simulation purposes.  This function set allows
// read and write access to the values in the cells of the fluid object.
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MFnDagNode.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MVectorArray;
class MDoubleArray;
class MColor;
class MPoint;

// *****************************************************************************
                                                 
// CLASS DECLARATION (MFnFluid)

/// Fluid node function set (OpenMayaFX)
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnFluid : public MFnDagNode 
{
	declareDagMFn(MFnFluid, MFnDagNode);
 
    ///
    enum FluidMethod {
        /// all values in grid are zero
        kZero,
        /// values in the grid are static
        kStaticGrid,
        /// values in the grid come from a dynamic solver
        kDynamicGrid,
        /// ramps the value based on the gradient setting
        kGradient
    };
    ///
    enum FluidGradient {
        /// value is set to one across the volume
        kConstant,
        /// ramp the value from zero to one along the X axis
        kXGradient,
        /// ramp the value from zero to one along the Y axis
        kYGradient,
        /// ramp the value from zero to one along the Z axis
        kZGradient,
        /// ramp the value from one to zero along the X axis
        kNegXGradient,
        /// ramp the value from one to zero along the Y axis
        kNegYGradient,
        /// ramp the value from one to zero along the Z axis
        kNegZGradient,
        /// ramps the value from one at the center to zero at the edges
        kCenterGradient
    };

	///
	enum FalloffMethod { 
		/// No falloff grid should be defined. 
		kNoFalloffGrid, 
		/// Values in the falloff grid are static. 
		kStaticFalloffGrid 
	}; 
		
    ///
    enum ColorMethod {
        /// off, use shading color instead
        kUseShadingColor,
        /// values in the grid are static
        kStaticColorGrid,
        /// values in the grid come from a dynamic solver
        kDynamicColorGrid
    };
    ///
    enum CoordinateMethod {
        /// values are equal the object space coordinates
        kFixed,
        /// coordinate values will be moved using the current density solver
        kGrid
    };
    
    // Create 2D and 3D fluids 
	///
    MObject create3D			( unsigned Xres, unsigned Yres, unsigned Zres, 
							      double Xdim, double Ydim, double Zdim, 
								  MObject parentOrOwner = MObject::kNullObj, 
								  MStatus * ReturnStatus = NULL ); 
  
	///
    MObject	create2D			( unsigned Xres,  unsigned Yres, 
							      double Xdim, double Ydim, 
							      MObject parentOrOwner = MObject::kNullObj, 
							      MStatus * ReturnStatus = NULL ); 

    // get and set resolution, dimensions 
	///
    MStatus getResolution		( unsigned &Xres, unsigned &Yres, 
            					  unsigned &Zres ) const; 
	///
    MStatus getResolution		( unsigned &Xres, unsigned &Yres ) const;
	///
    MStatus getDimensions		( double &Xdim, double &Ydim, 
            					  double &Zdim ) const; 
	///
    MStatus getDimensions		( double &Xdim, double &Ydim ) const; 
	///
    MStatus setSize				( unsigned Xres,  unsigned Yres, unsigned Zres, 
								  double Xdim,  double Ydim, double Zdim,
            					  bool resample ); 
	///
    MStatus setSize				( unsigned Xres,  unsigned Yres,
								  double Xdim,  double Ydim,
            					  bool resample ); 
    ///
    unsigned gridSize			( MStatus * ReturnStatus = NULL );
	///
    MStatus velocityGridSizes	( int & xsize, int & ysize, int & zsize );
  
    // get pointers to actual data area 
    // users may get and set data into these arrays, 
	///
	float * falloff				( MStatus * ReturnStatus = NULL ); 
	/// 
    float * density				( MStatus * ReturnStatus = NULL ); 
	///
    MStatus getVelocity			( float *& Xvel, float *& Yvel, float *& Zvel ); 
	///
    float * pressure			( MStatus * ReturnStatus = NULL ); 
	///
    float * temperature			( MStatus * ReturnStatus = NULL ); 
	///
    float * fuel				( MStatus * ReturnStatus = NULL ); 
	///
    MStatus getCoordinates		( float *& u, float *& v, float *& w ); 
	///
    MStatus getColors			( float *& r, float *& g, float *& b ); 

    // Set Data Modes 
	/// 
	MStatus setFalloffMode		( FalloffMethod mode ); 
	/// 
	MStatus getFalloffMode		( FalloffMethod &mode ); 
	
	///
    MStatus setDensityMode		( FluidMethod mode, FluidGradient gradient ); 
	///
    MStatus getDensityMode		( FluidMethod &mode, FluidGradient &gradient ); 
  
	///
    MStatus setVelocityMode		( FluidMethod mode, FluidGradient gradient ); 
	///
    MStatus getVelocityMode		( FluidMethod &mode, FluidGradient &gradient ); 
  
	///
    MStatus setTemperatureMode	( FluidMethod mode, FluidGradient gradient );
	/// 
    MStatus getTemperatureMode	( FluidMethod &mode, FluidGradient &gradient ); 

	///
    MStatus setFuelMode			( FluidMethod mode, FluidGradient gradient );
	/// 
    MStatus getFuelMode			( FluidMethod &mode, FluidGradient &gradient ); 

	///
    MStatus setCoordinateMode	( CoordinateMethod mode ) ; 
	///
    MStatus getCoordinateMode	( CoordinateMethod & mode ); 
  
	///
    MStatus setColorMode		( ColorMethod mode ); 
	///
    MStatus getColorMode		( ColorMethod & mode ); 
    
    ///
    MStatus getForceAtPoint(const MPointArray&   point, 
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    ///
    MStatus getForceAtPoint(const MVectorArray&  point, 
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
							double deltaTime = 1.0 / 24.0 );
    ///
	bool	toGridIndex( const MPoint& objectSpacePoint,
						 int3& gridCoords,
						 MStatus *status = NULL );
    ///
	MStatus voxelCenterPosition(int xi,
								int yi,
								int zi,
								MPoint& objectSpacePoint);

    ///
	MStatus	updateGrid			();
    
	///
	void emitIntoArrays( float val, 
						 int x, 
						 int y, 
						 int z, 
						 float density, 
						 float heat, 
						 float fuel, 
						 bool doColor, 
						 const MColor& emitColor );

    
    // convert to/from single index 
	///
    int     index				( int xi, int yi ); 
	///
    int     index				( int xi, int yi, int zi ); 
	///
    void	index				( int ai, int & xi, int & yi, int & zi ); 
	///
    int     index				( int xi, int yi, int zi,
								  int xres, int yres, int zres ); 
	///
    void	index				( int ai, int xres, int yres, int zres,
								  int & xi, int & yi, int & zi ); 

protected: 

  	virtual bool	objectChanged		( MFn::Type, MStatus * );

	void * 			updateGeomPtr		();
	void *			updateConstGeomPtr	() const;
    
private:
    MObject		  	create				( MObject parent, bool is2d,
        								  MStatus * );

	unsigned fNx, fNy, fNz;
};
    
#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnFluid */
