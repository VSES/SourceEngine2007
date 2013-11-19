#pragma once
#ifndef PCH_H
#define PCH_H
//var unreferenced
#pragma warning(disable : 4101)
//var init but unused
#pragma warning(disable : 4189)
//unreachable code
#pragma warning(disable : 4702)
//nonstandard extensions
#pragma warning(disable : 4238)

#include "windows.h"
#include "windowsx.h"
#include "Commdlg.h"
#include "process.h"
#include "shlwapi.h"
#include "comdef.h"

#include "atlstr.h"
#include <atlcoll.h>
#include <vector>
#include <bitset>

#include "stdio.h"
#include <iostream>

#undef STRICT
#pragma warning(disable : 4995)
#pragma warning(disable : 4996)
#pragma warning(disable : 4324)
#include "dxstdafx.h"

#include "d3d9.h"
#include "d3dx9.h"
#include "dxcc.h"






#ifndef REQUIRE_IOSTREAM
#define REQUIRE_IOSTREAM
#endif 

#ifndef NT_PLUGIN
#define NT_PLUGIN
#endif 

using namespace std;

#include <maya/MIOStream.h>
#include "maya/MPxNode.h"
#include "maya/MPxFileTranslator.h"
#include "maya/MPxTransform.h"
#include "maya/MPxCommand.h"
#include "maya/MArgList.h"
#include "maya/MStatus.h"
#include "maya/MFn.h"
#include "maya/M3dView.h"
#include "maya/MGlobal.h"
#include "maya/MAnimControl.h"
#include "maya/MSelectionList.h"
#include "maya/MItSelectionList.h"

#include "maya/MPlugArray.h"
#include "maya/MPlug.h"
#include "maya/MObject.h"
#include "maya/MObjectArray.h"
#include "maya/MFnDependencyNode.h"
#include "maya/MFnDagNode.h"
#include "maya/MDagPath.h"
#include "maya/MDagPathArray.h"
#include "maya/MObjectHandle.h"
#include "maya/MFnMesh.h"
#include "maya/MFnCamera.h"
#include "maya/MFnTransform.h"
#include "maya/MFnSkinCluster.h"
#include "maya/MFnSingleIndexedComponent.h"

#include "maya/MTypeId.h"
#include "maya/MDataHandle.h"
#include "maya/MDataBlock.h"
#include "maya/MFileObject.h"
#include "maya/MTime.h"
#include "maya/MIntArray.h"
#include "maya/MPoint.h"
#include "maya/MPointArray.h"
#include "maya/MVector.h"
#include "maya/MFloatVector.h"
#include "maya/mdoublearray.h"
#include "maya/MString.h"
#include "maya/MMatrix.h"
#include "maya/MFloatMatrix.h"

#include "maya/MFnNumericAttribute.h"
#include "maya/MFnTypedAttribute.h"
#include "maya/MFnCompoundAttribute.h"

#include "maya/mfndata.h"
#include "maya/MFnDoubleArrayData.h"
#include "maya/MFnNumericData.h"
#include "maya/MFnStringData.h"
#include "maya/MFnMatrixData.h"
#include "maya/MFnMeshData.h"

#include "maya/MItDependencyGraph.h"
#include "maya/MItDependencyNodes.h"
#include "maya/MItDag.h"
#include "maya/MItMeshPolygon.h"
#include "maya/MItMeshVertex.h"
#include "maya/MItDependencyGraph.h"

#include "maya/MSceneMessage.h"
#include "maya/MDGMessage.h"
#include "maya/MMessage.h"
#include "maya/MNodeMessage.h"
#include "maya/MDagMessage.h"
#include "maya/MEventMessage.h"









//ABSTRACT-CORE-START
#include "Manager.h"
#include "ManagerEvents.h"
#include "Viewer.h"
#include "ViewerEvents.h"
#include "Engine.h"
#include "EngineEvents.h"
#include "ManagerLock.h"
#include "Timestamp.h"
#include "RBMapIterator.h"
//ABSTRACT-CORE-END

//CONCRETE-CORE-START
#include "globals.h"
#include "DXUTEffectMap.h"
#include "MayaEngine.h"
#include "MayaViewer.h"
//#include "Tag.h"
//#include "TagContainer.h"
//#include "MeshExtractor.h"
#include "DXMGraph.h"
#include "DXMGraphAdapter.h"
#include "DXMGraphLayerAdapter.h"
#include "DXMGraphDagAdapter.h"
#include "DXMGraphMeshAdapter.h"
#include "DXMGraphShaderAdapter.h"
#include "DXMGraphStdMtlAdapter.h"
#include "DXMAnchor.h"
#include "DXMNode.h"
#include "DXMNodeAdapter.h"
#include "DXMNodeLayerAdapter.h"
#include "DXMNodeDagAdapter.h"
#include "DXMNodeMeshAdapter.h"
#include "DXMNodeShaderAdapter.h"
#include "DXMNodeStdMtlAdapter.h"
#include "DXMRoute.h"
#include "MayaManager.h"
#include "DXMUnitTests.h"
#include "FxShader.h"
#include "FxInternal.h"
#include "FxAttributeCreator.h"
#include "FxAttributeFiller.h"
#include "FileTranslator.h"
#include "exportcommands.h"
#include "previewcommands.h"
#include "rebuildcommands.h"
#include "viewcommands.h"
#include "helpcommands.h"
//CONCRETE-CORE-END





#endif