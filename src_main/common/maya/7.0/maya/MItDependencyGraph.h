
#ifndef _MItDependencyGraph
#define _MItDependencyGraph
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
// CLASS:    MItDependencyGraph
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItDependencyGraph)
//
//	In Maya, all geometry, animation and rendering information is implemented
// 	in Nodes in the Dependency Graph (DG).  The DG includes the Directed Acyclic
//	Graph (DAG).  Therefore, all DAG Nodes are also DG Nodes.  The data on Nodes
//	is associated with Attributes.  Attributes on Nodes are connected to
//	Attributes on other Nodes via Plugs on the Attributes.  Plugs are, in effect
//	the external intefaces of Attributes.
//
//	The DG Iterator Class (MItDependencyGraph) provides methods for iterating
//	over either Nodes or Plugs, as well as methods for setting and querying the
//	characteristics and behaviour of the iterator.
//
//  This iterator will traverse all attributes upstream or downstream from the
//  root node of the traversal. For non root nodes, only attributes that are
//  affected by the incoming attribute to that node will be traversed.
//  Hence, only nodes to which data from the root node is flowing will be
//  traversed.
// 
//	The DG Iterator is used in conjunction with the Maya Object (MObject), Plug
//	(MPlug), Maya Object Array (MObjectArray) and Plug Array (MPlugArray)
//	classes.
//
//	It is also useful to use Function Sets specific to the Nodes returned by
//	the iterator to query and modify the Nodes in the DG.
//
//	The DG itself can be modified using a DG Modifer (MDGModifier).
//
//	Additionally, Nodes can be added to and retrieved from selection lists using
//	the Selection List (MSelectionList) class and Selection List Iterator
//	(MItSelectionList).  This can be useful for obtaining the root Node for an
//	iteration.
//
//	Attributes on the Nodes can be manipulated using the Attribute Function Set
//	(MFnAttribute) and its derivations.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFn.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MItDependencyGraph)

class MIteratorType;

/// Dependency Graph Iterator
/**

  Iterate over Dependency Graph (DG) Nodes or Plugs starting at a specified
  root Node or Plug.

  Set and query the root of the iteration.

  Set and query the direction (downstream or upstream), traversal priority
  (depth first or breadth first) and level of detail (Node level or Plug
  level) of the iteration.

  Set and disable a filter to iterate over only speicifc types (MFn::Type) of
  Nodes.

  Reset the root, filter, direction, traversal priority and level of detail
  of the iteration.

  Prune branches of the graph from iteration.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItDependencyGraph  
{
public:
				/// Direction within the DG relative to root Node or Plug
	            enum	Direction
				{
					/// From source to destination
					kDownstream,
					/// From destination to source
					kUpstream
				};

				/// Perform a depth first or breadth first traversal
	            enum	Traversal
				{
					/// Away from root first
					kDepthFirst,
					/// Equidistant from root first
					kBreadthFirst
				};

				/// Level of detail of the iteration
                enum	Level
				{
					///  Visit each Node at most once
					kNodeLevel,
					/// Visit each Plug at most once
					kPlugLevel
				};
	///
				MItDependencyGraph ( MObject& rootNode,
									 MFn::Type filter = MFn::kInvalid,
									 Direction direction = kDownstream,
									 Traversal traversal = kDepthFirst,
									 Level level = kNodeLevel,
									 MStatus* ReturnStatus = NULL );
	///
				MItDependencyGraph ( MPlug& rootPlug,
									 MFn::Type filter = MFn::kInvalid,
									 Direction direction = kDownstream,
									 Traversal traversal = kDepthFirst,
									 Level level = kPlugLevel,
									 MStatus* ReturnStatus = NULL );
	///
				MItDependencyGraph( MObject& rootNode,
									MPlug& rootPlug,
									MIteratorType& infoObject,
									Direction direction = kDownstream,
									Traversal traversal = kDepthFirst,
									Level level = kNodeLevel,
									MStatus* ReturnStatus = NULL );
	///
	            ~MItDependencyGraph ();
	///
	MStatus		reset();
	///
	MStatus		resetTo( MObject& rootNode,
						 MFn::Type filter = MFn::kInvalid,
						 Direction direction = kDownstream,
						 Traversal traversal = kDepthFirst,
						 Level level = kNodeLevel );
	///
	MStatus		resetTo( MPlug& rootPlug,
						 MFn::Type filter = MFn::kInvalid,
						 Direction direction = kDownstream,
						 Traversal traversal = kDepthFirst,
						 Level level = kPlugLevel );
	///
	MStatus		resetTo( MObject* rootNode,
						 MPlug* rootPlug,
						 MIteratorType& infoObject,
						 Direction direction = kDownstream,
						 Traversal traversal = kDepthFirst,
						 Level level = kPlugLevel );
	///
	MObject		rootNode( MStatus* ReturnStatus = NULL );
	///
	MPlug		rootPlug( MStatus* ReturnStatus = NULL );
	///
	MFn::Type	currentFilter( MStatus* ReturnStatus = NULL );
	///
	MStatus		setCurrentFilter( MFn::Type filter = MFn::kInvalid );
	///
	MStatus		resetFilter();
	///
	bool		isPruningOnFilter( MStatus* ReturnStatus = NULL );
	///
	MStatus		enablePruningOnFilter();
	///
	MStatus		disablePruningOnFilter();
	///
	bool		isDirectionDownStream(  MStatus* ReturnStatus = NULL );
	///
	Direction	currentDirection(   MStatus* ReturnStatus = NULL );
	///
	MStatus		toggleDirection( );
	///
	bool		isTraversalDepthFirst(  MStatus* ReturnStatus = NULL );
	///
	Traversal	currentTraversal(   MStatus* ReturnStatus = NULL );
	///
	MStatus		toggleTraversal( );
	///
	bool		atNodeLevel( MStatus* ReturnStatus = NULL );
	///
	Level		currentLevel(   MStatus* ReturnStatus = NULL );
	///
	MStatus		toggleLevel( );
	///
	MStatus		next( );
	///
	bool		isDone( MStatus* ReturnStatus = NULL );
	///
	MStatus		prune( );
	///
	MObject		thisNode( MStatus* ReturnStatus = NULL );
	///
	bool		thisNodeHasUnknownType( MStatus* ReturnStatus = NULL );
	///
	MPlug		thisPlug( MStatus* ReturnStatus = NULL );
	///
	MPlug		previousPlug( MStatus* ReturnStatus = NULL );
	///
	MStatus		getNodesVisited( MObjectArray& nodesVisted ) const;
	///
	MStatus		getPlugsVisited( MPlugArray& plugsVisted ) const;
	///
	MStatus		getNodePath( MObjectArray& path ) const;
	///
	MStatus		getPlugPath( MPlugArray& path ) const;

protected:
// No protected members

private:
	static const char*		className();
	void *					currentIterator;

	bool					objectsHasFnOnFilterOrList(const MObject& nodeObj, 
													   const MObject& attrObj);

	void					getCurrentIteratorFromMObjectOrMPlug(MObject& rootNode, 
																 MPlug&   rootPlug, 
																 Direction direction,
																 Traversal traversal,
																 Level level,
																 MStatus* ReturnStatus);

	void					resetCurrentIteratorFromMObjectOrMPlug(MObject& rootNode, 
															  MPlug&   rootPlug, 
															  Direction direction,
															  Traversal traversal,
															  Level level,
															  MStatus* ReturnStatus);

	MFn::Type				currentTypeFilter;
	bool					pruneOnNoMatch;

	MIntArray				filterList;
	bool					useFilterList;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItDependencyGraph */
