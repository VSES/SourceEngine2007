/*
//GRAPH//
Does graph have graph adapter?
List all of a graph's adapters.

//NODES//
Does graph track a node?
Does graph's node have a particular node adapter?
List all of a graph's node's adapters.
Validate that each Node
	has atleast 1 node adapter
	that each node adapter's owner is interested in it.

//ROUTES//
Does the graph have a Route marker for a specific Dagpath
Validate that each DagPath's 
	Route markers are named appropriately
	Route markers are parented appropriately
	Route markers' frame is parented appropriately
	Route markers are childed appropriately
	Route markers' frame is childed appropriately
	Route markers' has no unknown children
	Route markers' frame has no unknown children
*/


//pounding tests...load unload files N times
//
/*
//scriptjob 
	-parent MENUITEM
	-replacePrevious
	-killWithScene
	-runOnce false
	-compressUndo true
	//-attributeChange $dxfxFileAttribute "refreshAE;";
*/


/*
//scriptjob -event "deleteAll";

//scriptjob -event "NewSceneOpened";
//scriptjob -event "SceneOpened";

//scriptjob -event "quitApplication";


//scriptjob -event "timeChanged";

//scriptjob -event "busy";//not sure how this is used

//scriptjob -event "flushingScene";

//scriptjob -event "readingFile";







global proc miGlobalsTextIncludeReplace(string $attr)
{
	string	$menu = "textIncludeMenu";

	optionMenu -e -cc ("miGlobalsEditText " + $menu + " " + $attr) $menu;
	symbolButton -e -c ("miGlobalsNewText 0 " + $attr) newTextIncludeButton;

	miGlobalsTextMenuUpdate 0 $menu $attr;

	scriptJob -p $menu -rp -ac $attr
		("miGlobalsTextMenuUpdate 0 " + $menu + " " + $attr);
}



*/











class MFnPlugin;

class DXMUnitTests
{
public:
	typedef CRBMap< CStringA , DXMGraph*, CStringElementTraits<CStringA> > DXMGraphMap;

	static DXMGraphMap Graphs;
	static MStringArray Commands;

	static void RegisterCommands( MFnPlugin& plugin );
	static void RegisterCommand( MFnPlugin& plugin, const MString& commandName, MCreatorFunction creatorFunction );
	static void DeregisterCommands( MFnPlugin& plugin);

	static DXMGraph* GetGraph( MString& name);
	static DXMGraphAdapter* GetGraphAdapter( DXMGraph* graph, MString& graph_adapter);
	static DXMNode* GetNode( DXMGraph* graph, MString& node_name);
	static DXMNodeAdapter* GetNodeAdapter( DXMNode* node, MString& node_adapter_name);
	static DXMRoute* GetRoute( DXMGraph* graph, MString& dagpath_fullpathname);
	static DXMRoute* GetRouteOnNode( DXMNodeAdapter* nodeAdapter, MString& dagpath_fullpathname);

	static MDagPath GetPath( MString& dagpath_fullpathname);
	static MObject GetObject( MString& object_name);
	static MString GetPathName( MDagPath& dagpath);
	static MString GetObjectName( MObject& obk);

};


class DXMListCommands : public MPxCommand 
{							
public:		
	DXMListCommands(){};						
	static void*	creator(){ return new DXMListCommands; }									
	virtual MStatus	doIt ( const MArgList& args);		
};	

class DxMPxCommand : public MPxCommand 
{							
public:	
	DxMPxCommand()
	{
		ArgCount= -1;
		Help= "No help was provided for this command.";
	}

	virtual MStatus	doIt ( const MArgList& args);

protected:
	virtual MStatus	Run(const MArgList& args)= 0;		

	CStringA Help;
	int ArgCount;//-1 == any
};

class DXMDebugDxccToggleCommand : public DxMPxCommand 
{							
public:		
	DXMDebugDxccToggleCommand() 	
	{
		ArgCount= 0;
		Help= "No Arguments";
	}

	static bool ToggleState;							

	static void*	creator(){ return new DXMDebugDxccToggleCommand; }									
	virtual MStatus	Run ( const MArgList& args) 
	{ 
		ToggleState= !ToggleState;

		if(ToggleState)
		{
			DXCCSetDebugPrintfACallback( MayaDebugPrintfACallback ) ;
			DXCCSetDebugPrintfWCallback( MayaDebugPrintfWCallback ) ;
		}
		else
		{
			DXCCSetDebugPrintfACallback( NULL ) ;
			DXCCSetDebugPrintfWCallback( NULL ) ;
		}	

		return MStatus::kSuccess; 
	}		
};	

class DXMDebugBasicToggleCommand : public DxMPxCommand 
{							
public:		
	DXMDebugBasicToggleCommand() 	
	{
		ArgCount= 0;
		Help= "No Arguments";
	}
								
	static void*	creator(){ return new DXMDebugBasicToggleCommand; }									
	virtual MStatus	Run ( const MArgList& args) { g_DebugBasic = !g_DebugBasic; return MStatus::kSuccess; }		
};	

class DXMDebugExtremeToggleCommand : public DxMPxCommand 
{							
public:		
	DXMDebugExtremeToggleCommand() 	
	{
		ArgCount= 0;
		Help= "No Arguments";
	}
								
	static void*	creator(){ return new DXMDebugExtremeToggleCommand; }									
	virtual MStatus	Run ( const MArgList& args) { g_DebugExtreme = !g_DebugExtreme; return MStatus::kSuccess; }		
};	



class DXMListGraphsCommand : public DxMPxCommand 
{							
public:		
	DXMListGraphsCommand() 	
	{
		ArgCount= 0;
		Help= "No Arguments";
	}
								
	static void*	creator(){ return new DXMListGraphsCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMGraphListAdaptersCommand : public DxMPxCommand 
{							
public:		
	DXMGraphListAdaptersCommand() 	
	{
		ArgCount= 1;
		Help= "(stringarray)result command (string)graph";
	}

	static void*	creator(){ return new DXMGraphListAdaptersCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMGraphHasAdapterCommand : public DxMPxCommand 
{							
public:		
	DXMGraphHasAdapterCommand() 	
	{
		ArgCount= 2;
		Help= "(stringarray)result command (string)graph (string)graphAdapter";
	}

	static void*	creator(){ return new DXMGraphHasAdapterCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMGraphAdapterIsInterestedCommand : public DxMPxCommand 
{							
public:		
	DXMGraphAdapterIsInterestedCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)graphAdapter (string)node";
	}

	static void*	creator(){ return new DXMGraphAdapterIsInterestedCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMGraphListNodesCommand : public DxMPxCommand 
{							
public:		
	DXMGraphListNodesCommand() 	
	{
		ArgCount= 1;
		Help= "(stringarray)result command (string)graph";
	}

							
	static void*	creator(){ return new DXMGraphListNodesCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMGraphHasInvalidNodesCommand : public DxMPxCommand 
{							
public:		
	DXMGraphHasInvalidNodesCommand() 	
	{
		ArgCount= 1;
		Help= "(stringarray)result command (string)graph";
	}

							
	static void*	creator(){ return new DXMGraphHasInvalidNodesCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMGraphHasNodeCommand : public DxMPxCommand 
{							
public:		
	DXMGraphHasNodeCommand() 	
	{
		ArgCount= 2;
		Help= "(stringarray)result command (string)graph (string)node";
	}

							
	static void*	creator(){ return new DXMGraphHasNodeCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMNodeCountAdaptersOwnedByCommand : public DxMPxCommand 
{							
public:		
	DXMNodeCountAdaptersOwnedByCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)node (string)graphAdapter";
	}

							
	static void*	creator(){ return new DXMNodeCountAdaptersOwnedByCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeListAdaptersCommand : public DxMPxCommand 
{							
public:		
	DXMNodeListAdaptersCommand() 	
	{
		ArgCount= 2;
		Help= "(stringarray)result command (string)graph (string)node\nnode must be qualified if not unique";
	}

								
	static void*	creator(){ return new DXMNodeListAdaptersCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeHasAdapterCommand : public DxMPxCommand 
{							
public:		
	DXMNodeHasAdapterCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)node (string)nodeAdapter\nnode must be qualified if not unique";
	}

								
	static void*	creator(){ return new DXMNodeHasAdapterCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeAdapterGetOwnerCommand : public DxMPxCommand 
{							
public:		
	DXMNodeAdapterGetOwnerCommand() 	
	{
		ArgCount= 3;
		Help= "(string)result command (string)graph (string)node (string)nodeAdapter\nnode must be qualified if not unique";
	}

							
	static void*	creator(){ return new DXMNodeAdapterGetOwnerCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMNodeDagIsExplicitlyInvisibleCommand : public DxMPxCommand 
{							
public:		
	DXMNodeDagIsExplicitlyInvisibleCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)node\nnode must be qualified if not unique";
	}

								
	static void*	creator(){ return new DXMNodeDagIsExplicitlyInvisibleCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeDagGetLocalMatrixCommand : public DxMPxCommand 
{							
public:		
	DXMNodeDagGetLocalMatrixCommand() 	
	{
		ArgCount= 2;
		Help= "(doublearray)result command (string)graph (string)node\nnode must be qualified if not unique";
	}

								
	static void*	creator(){ return new DXMNodeDagGetLocalMatrixCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMGraphListRoutesCommand : public DxMPxCommand 
{							
public:		
	DXMGraphListRoutesCommand() 	
	{
		ArgCount= 1;
		Help= "(stringarray)result command (string)graph";
	}

								
	static void*	creator(){ return new DXMGraphListRoutesCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMGraphHasRouteCommand : public DxMPxCommand 
{							
public:		
	DXMGraphHasRouteCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)path";
	}
							
	static void*	creator(){ return new DXMGraphHasRouteCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMGraphHasInvalidRoutesCommand : public DxMPxCommand 
{							
public:		
	DXMGraphHasInvalidRoutesCommand() 	
	{
		ArgCount= 1;
		Help= "(int)result command (string)graph";
	}

								
	static void*	creator(){ return new DXMGraphHasInvalidRoutesCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMNodeDagListRoutesCommand : public DxMPxCommand 
{							
public:		
	DXMNodeDagListRoutesCommand() 	
	{
		ArgCount= 2;
		Help= "(stringarray)result command (string)graph (string)node\nnode must be qualified if not unique";
	}

								
	static void*	creator(){ return new DXMNodeDagListRoutesCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeDagHasRouteCommand : public DxMPxCommand 
{							
public:		
	DXMNodeDagHasRouteCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)path (string)node";
	}

								
	static void*	creator(){ return new DXMNodeDagHasRouteCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMRouteGetMemberCountCommand : public DxMPxCommand 
{							
public:		
	DXMRouteGetMemberCountCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteGetMemberCountCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	


class DXMRouteGetParentCommand : public DxMPxCommand 
{							
public:		
	DXMRouteGetParentCommand() 	
	{
		ArgCount= 2;
		Help= "(string)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteGetParentCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMRouteListChildrenCommand : public DxMPxCommand 
{							
public:		
	DXMRouteListChildrenCommand() 	
	{
		ArgCount= 2;
		Help= "(stringarray)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteListChildrenCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMRouteHasChildCommand : public DxMPxCommand 
{							
public:		
	DXMRouteHasChildCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)path (string)childPath ";
	}

								
	static void*	creator(){ return new DXMRouteHasChildCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMRouteGetNodeCommand : public DxMPxCommand 
{							
public:		
	DXMRouteGetNodeCommand() 	
	{
		ArgCount= 2;
		Help= "(string)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteGetNodeCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	
class DXMRouteIsFrameParentingEqualCommand : public DxMPxCommand 
{							
public:		
	DXMRouteIsFrameParentingEqualCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteIsFrameParentingEqualCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	


class DXMRouteIsVisibleCommand : public DxMPxCommand 
{							
public:		
	DXMRouteIsVisibleCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteIsVisibleCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	


class DXMRouteGetLocalMatrixCommand : public DxMPxCommand 
{							
public:		
	DXMRouteGetLocalMatrixCommand() 	
	{
		ArgCount= 2;
		Help= "(doublearray)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteGetLocalMatrixCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMRouteGetWorldMatrixCommand : public DxMPxCommand 
{							
public:		
	DXMRouteGetWorldMatrixCommand() 	
	{
		ArgCount= 2;
		Help= "(doublearray)result command (string)graph (string)path";
	}

								
	static void*	creator(){ return new DXMRouteGetWorldMatrixCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};	

class DXMNodeMeshIsAnyRouteVisibleCommand : public DxMPxCommand 
{							
public:		
	DXMNodeMeshIsAnyRouteVisibleCommand() 	
	{
		ArgCount= 2;
		Help= "(int)result command (string)graph (string)node";
	}

								
	static void*	creator(){ return new DXMNodeMeshIsAnyRouteVisibleCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};

class DXMNodeMeshIsMemberOfRouteCommand : public DxMPxCommand 
{							
public:		
	DXMNodeMeshIsMemberOfRouteCommand() 	
	{
		ArgCount= 3;
		Help= "(int)result command (string)graph (string)node (string)path";
	}

								
	static void*	creator(){ return new DXMNodeMeshIsMemberOfRouteCommand; }									
	virtual MStatus	Run ( const MArgList& args);		
};