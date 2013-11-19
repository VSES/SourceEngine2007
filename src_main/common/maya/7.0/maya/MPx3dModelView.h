#ifndef _MPx3dModelView
#define _MPx3dModelView
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

#if defined __cplusplus

// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/M3dView.h>

// DECLARATIONS

// CLASS DECLARATION (MPx3dModelView)

class MDagPath;
class MSelectionList;

/// 3d Model View (MPx3dModelView)
/**
	Class for creating custom model views
*/

class OPENMAYAUI_EXPORT MPx3dModelView {
public: 
	///
	enum LightingMode {
		///	
		kLightAll,
		///
		kLightSelected,
		///
		kLightActive,
		///
		kLightDefault,
		///
		kLightNone,
		///
		kLightQuality
	};
	///
	enum FogSource {
		///	fog is computed per pixel (default)
		kFogFragment,
		///	fog is computed by specified vertex fog coordinates
		kFogCoordinate
	};
	///
	enum FogMode {
		///	linear drop off
		kFogLinear,
		///	exponential drop-off
		kFogExponential,
		///	squared exponential drop-off
		kFogExponentialSquared
	};

	///
						MPx3dModelView();
	///
	virtual				~MPx3dModelView();

	///
			MString		name(MStatus *ReturnStatus = NULL) const;
	///
	virtual	MString		viewType() const;
	///
	virtual void		preMultipleDraw();
	///
	virtual void		postMultipleDraw();
	///
	virtual void		preMultipleDrawPass(unsigned index);
	///
	virtual void		postMultipleDrawPass(unsigned index);
	///
	virtual bool		okForMultipleDraw(const MDagPath &);
	///
	virtual unsigned 	multipleDrawPassCount();
	///
	bool				multipleDrawEnabled() const;
	///
	void				setMultipleDrawEnable(bool enable);
	///		
	MStatus				updateViewingParameters();

	///
	virtual void		removingCamera(MDagPath &cameraPath);

	///
			MStatus		refresh(bool all = false, bool force = false);

	//	Text methods
	//
	///
	MStatus				drawText(const MString &text, const MPoint position,
								 M3dView::TextPosition textPosition 
								 = M3dView::kLeft);

	//
	//	OpenGL wrapper methods
	//
	///
	MStatus				beginGL();
	///
	MStatus				endGL();

	//	Camera methods
	//
	///					
	MStatus 			setCameraInDraw(MDagPath & camera);
	///	
	MStatus 			setCamera(MDagPath & camera);
	///
	MStatus				getCamera(MDagPath & camera);

	//	Heads Up Display Methods
	//
	///
	virtual MString		getCameraHUDName();
	///
	MStatus 			setDisplayHUD(bool display);
	///
	bool				displayHUD(MStatus *ReturnStatus = NULL) const;

	///
	MStatus			setDisplayAxis(bool display);
	///
	bool			displayAxisOn(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setDisplayAxisAtOrigin(bool display);
	///
	bool			displayAxisAtOriginOn(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setDisplayCameraAnnotation(bool display);
	///
	bool		displayCameraAnnotationOn(MStatus *ReturnStatus = NULL) const;

	///
	bool			isVisible(MStatus *ReturnStatus = NULL) const;

	//	Display style methods
	//
	///
	M3dView::DisplayStyle	displayStyle(MStatus *ReturnStatus = NULL) const;
	///
	bool			isShadeActiveOnly(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setDisplayStyle(M3dView::DisplayStyle style,
									bool activeOnly = false);

	///
	int				portWidth( MStatus * ReturnStatus = NULL );
	///
	int				portHeight( MStatus * ReturnStatus = NULL );

	//	Overlay plane methods
	//

	///
	MStatus			beginOverlayDrawing();
	///
	MStatus			endOverlayDrawing();
	///
	MStatus			clearOverlayPlane();

	// Color methods
	//

	///
	MStatus			setDrawColor(unsigned index,
						M3dView::ColorTable table = M3dView::kActiveColors );
	///
	MStatus			setDrawColor(const MColor & color);

	///	
	bool			isColorIndexMode(MStatus * ReturnStatus = NULL);
	///
	MStatus			setColorIndexMode(bool state);
	///
	unsigned		numDormantColors(MStatus * ReturnStatus = NULL );
	///
	unsigned 		numActiveColors(MStatus * ReturnStatus = NULL );
	///
	unsigned 		numUserDefinedColors(MStatus * ReturnStatus = NULL);

	///
	MStatus			setUserDefinedColor(unsigned index, const MColor & color);
	///
	unsigned		userDefinedColorIndex(unsigned index,
										  MStatus * ReturnStatus = NULL );

	///
	MColor			templateColor(MStatus * ReturnStatus = NULL);
	///
	MColor			backgroundColor(MStatus * ReturnStatus = NULL);

	///
	MColor 			colorAtIndex(unsigned index, M3dView::ColorTable table 
								 = M3dView::kActiveColors,
								 MStatus * ReturnStatus = NULL);
	///
	MStatus			getColorIndexAndTable(unsigned glindex, unsigned &index,
										  M3dView::ColorTable &table ) const;

	
	// Transformation methods
	//

	///
	MStatus			viewToWorld(short x_pos, short y_pos,
								MPoint & worldPt, MVector & worldVector ) const;
	///
	MStatus			viewToWorld(short x_pos, short y_pos,
								MPoint & nearClipPt, MPoint & farClipPt ) const;
	///
	MStatus			viewToObjectSpace(short x_pos, short y_pos,
									  const MMatrix & localMatrixInverse,
									  MPoint & oPt, MVector & oVector ) const;
	///
	bool			worldToView(const MPoint& worldPt,
								short& x_pos, short& y_pos,
								MStatus * ReturnStatus = NULL ) const;

	//	Exclude/display flags
	//
	///
	MStatus 		setObjectDisplay(M3dView::DisplayObjects, bool);
	///
	bool			objectDisplay(M3dView::DisplayObjects, 
								  MStatus *ReturnStatus = NULL);
	//	Culling flags
	//
	///
	MStatus			setBackfaceCulling(bool cull);
	///
	bool			isBackfaceCulling(MStatus *ReturnStatus = NULL) const;

	//	texture/display flags
	//
	///
	MStatus			setWireframeOnShaded(bool on);
	///
	bool			isWireframeOnShaded(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setXrayEnabled(bool xray);
	///
	bool			isXrayEnabled(MStatus *ReturnStatus = NULL) const;

	///
	MStatus			setTextureDisplayEnabled(bool texture);
	///
	bool			isTextureDisplayEnabled(MStatus *ReturnStatus = NULL) const;

	//	Lighting flags
	///
	MStatus			setTwoSidedLighting(bool twoSided);
	///
	bool			isTwoSidedLighting(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setLightingMode(MPx3dModelView::LightingMode);
	///
	MPx3dModelView::LightingMode   lightingMode(MStatus *ReturnStatus = NULL) const;

	// 	Fog
	///
	MStatus			setFogEnabled(bool state);
	///	
	bool			isFogEnabled(MStatus *ReturnStatus = NULL) const;
	///
	MPx3dModelView::FogSource	fogSource(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setFogSource(MPx3dModelView::FogSource);
	///
	MPx3dModelView::FogMode		fogMode(MStatus *ReturnStatus = NULL) const;
	///
	MStatus 		setFogMode(MPx3dModelView::FogMode);
	///
	double			fogDensity(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setFogDensity(double);
	///
	double			fogStart(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setFogStart(double);
	///
	double 			fogEnd(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setFogEnd(double);
	///
	MColor			fogColor(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setFogColor(const MColor &);
	///
	bool			isBackgroundFogEnabled(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setBackgroundFogEnabled(bool enable); 


	//	View Selected
	///
	MString			viewSelectedPrefix(MStatus *ReturnStatus) const;
	///
	MStatus			setViewSelectedPrefix(MString &prefix);
	///
	bool			viewSelected(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setViewSelected(bool viewSelected);

	///
	MObject			viewSelectedSet(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setViewSelectedSet(MObject &set);

	///
	MStatus			getObjectsToView(MSelectionList &list) const;
	///
	MStatus			setObjectsToView(const MSelectionList &list);

	///
	MStatus		getAsM3dView(M3dView &view);
	///
	static MPx3dModelView* getModelView(MString &name, 
										MStatus *ReturnStatus = NULL);
protected:
private:
	static const char*  className();
	void setData(void *ptr);
	void setModelEditorData(void *ptr);
	void *				instance;
	void *				panelData;
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPx3dModelView */
