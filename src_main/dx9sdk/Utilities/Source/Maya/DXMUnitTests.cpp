#include "pch.h"

//MFnPlugin should only be included once.  these define changes works around that problem
#ifdef NT_PLUGIN
#undef NT_PLUGIN
#endif 
#ifndef MNoVersionString
#define MNoVersionString
#endif 
#include <maya/MFnPlugin.h>


bool DXMDebugDxccToggleCommand::ToggleState= false;							
DXMUnitTests::DXMGraphMap DXMUnitTests::Graphs;
MStringArray DXMUnitTests::Commands;

void DXMUnitTests::DeregisterCommands( MFnPlugin& plugin )
{
	plugin.deregisterCommand("DXMListCommands");

	for( UINT i= 0; i < Commands.length(); i++)
	{
		plugin.deregisterCommand(Commands[i]);
	}
	Commands.clear();
}

void DXMUnitTests::RegisterCommands( MFnPlugin& plugin)
{
	MStatus stat;

	DXMUnitTests::Graphs.SetAt("DXMSyncGraph", &g_PreviewPipeline.TagGraph);


	plugin.registerCommand(					"DXMListCommands",						DXMListCommands::creator );

	DXMUnitTests::RegisterCommand(plugin, 	"DXMDebugDxccToggle",					DXMDebugDxccToggleCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMDebugBasicToggle",					DXMDebugBasicToggleCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMDebugExtremeToggle",				DXMDebugExtremeToggleCommand::creator );


	//Global
	DXMUnitTests::RegisterCommand(plugin, 	"DXMListGraphs",						DXMListGraphsCommand::creator );

	//Graph
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphHasInvalidRoutes",				DXMGraphHasInvalidRoutesCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphHasInvalidNodes",				DXMGraphHasInvalidNodesCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphListAdapters",					DXMGraphListAdaptersCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphHasAdapter",					DXMGraphHasAdapterCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphListNodes",					DXMGraphListNodesCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphHasNode",						DXMGraphHasNodeCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphListRoutes",					DXMGraphListRoutesCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphHasRoute",						DXMGraphHasRouteCommand::creator );

	//GraphAdapter
	DXMUnitTests::RegisterCommand(plugin, 	"DXMGraphAdapterIsInterested",			DXMGraphAdapterIsInterestedCommand::creator );

	//Node
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeListAdapters",					DXMNodeListAdaptersCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeHasAdapter",					DXMNodeHasAdapterCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeCountAdaptersOwnedBy",			DXMNodeCountAdaptersOwnedByCommand::creator );
	
	//NodeAdapter
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeAdapterGetOwner",				DXMNodeAdapterGetOwnerCommand::creator );

	//NodeDagAdapter
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeDagListRoutes",					DXMNodeDagListRoutesCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeDagHasRoute",					DXMNodeDagHasRouteCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeDagIsExplicitlyInvisible",		DXMNodeDagIsExplicitlyInvisibleCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeDagGetLocalMatrix",				DXMNodeDagGetLocalMatrixCommand::creator );

	//NodeMeshAdapter
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeMeshIsAnyRouteVisible",			DXMNodeMeshIsAnyRouteVisibleCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMNodeMeshIsMemberOfRoute",			DXMNodeMeshIsMemberOfRouteCommand::creator );
	
	//Route
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteGetMemberCount",				DXMRouteGetMemberCountCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteGetParent",					DXMRouteGetParentCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteListChildren",					DXMRouteListChildrenCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteHasChild",						DXMRouteHasChildCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteIsFrameParentingEqual",		DXMRouteIsFrameParentingEqualCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteGetNode",						DXMRouteGetNodeCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteIsVisible",					DXMRouteIsVisibleCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteGetLocalMatrix",				DXMRouteGetLocalMatrixCommand::creator );
	DXMUnitTests::RegisterCommand(plugin, 	"DXMRouteGetWorldMatrix",				DXMRouteGetWorldMatrixCommand::creator );

}

MStatus	DXMListCommands::doIt ( const MArgList& args)
{
	setResult( DXMUnitTests::Commands );
	return MStatus::kSuccess;
}


void DXMUnitTests::RegisterCommand( MFnPlugin& plugin, const MString& commandName, MCreatorFunction creatorFunction )
{
	Commands.append(commandName);
	DXCHECK_MSTATUS(plugin.registerCommand( commandName,  creatorFunction ));
}



DXMGraph* DXMUnitTests::GetGraph( MString& name)
{
	DXMGraph* graph= NULL;
	DXMUnitTests::Graphs.Lookup(name.asChar(), graph);
	return graph;
}

DXMGraphAdapter* DXMUnitTests::GetGraphAdapter( DXMGraph* graph, MString& graph_adapter_name)
{
	if(!graph)
		return NULL;

	return graph->FindAdapter(graph_adapter_name.asChar());
}


DXMNode* DXMUnitTests::GetNode( DXMGraph* graph, MString& node_name)
{
	if(!graph)
		return NULL;

	MObject obj= DXMUnitTests::GetObject(node_name);

	return graph->FindNode(obj);
}

DXMNodeAdapter* DXMUnitTests::GetNodeAdapter( DXMNode* node, MString& node_adapter_name)
{
	if(!node)
		return NULL;

	return node->FindAdapter( node_adapter_name.asChar() );
}

MDagPath DXMUnitTests::GetPath( MString& dagpath_fullpathname)
{

	MSelectionList Selected;
	Selected.add(dagpath_fullpathname);

	MDagPath path;
	Selected.getDagPath(0, path);

	return path;
}

DXMRoute* DXMUnitTests::GetRoute( DXMGraph* graph, MString& dagpath_fullpathname)
{
	MDagPath path= DXMUnitTests::GetPath(dagpath_fullpathname);
	if(!path.isValid())
		return NULL;

	DXMGraphDagAdapter* graphDagAdapter= reinterpret_cast<DXMGraphDagAdapter*>( graph->FindAdapter(DXMGraphDagAdapter::MySignature) );
	if(graphDagAdapter)
	{
		return graphDagAdapter->FindRoute(path);
	}

	return NULL;
}
	
MObject DXMUnitTests::GetObject( MString& object_name)
{
	LPCSTR name= object_name.asChar();

	MSelectionList Selected;
	Selected.add( object_name );


	MObject obj;
	Selected.getDependNode(0, obj);

	return obj;
}

DXMRoute* DXMUnitTests::GetRouteOnNode( DXMNodeAdapter* nodeAdapter, MString& dagpath_fullpathname)
{
	if(!nodeAdapter)
		return NULL;

	if( nodeAdapter->Signature() != DXMNodeDagAdapter::MySignature )
		return NULL;

	MDagPath path= DXMUnitTests::GetPath(dagpath_fullpathname);
	if(!path.isValid())
		return NULL;

	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>(nodeAdapter);

	for( DXMNodeDagAdapter::DXMRouteIterator routeItr= nodeDagAdapter->GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);

		if( route->GetPath() == path)
		{
			return route;
		}
	}

	return NULL;
}

MString DXMUnitTests::GetPathName( MDagPath& dagpath)
{
	//MString result= dagpath.fullPathName();
	//if(result.asChar()[0] == '|')
	//	result= result.substring(1, result.length()-1);

	return dagpath.fullPathName();
}

MString DXMUnitTests::GetObjectName( MObject& obj)
{
	MString result;
	if(obj.hasFn(MFn::kDagNode))
	{
		MFnDagNode dagNode(obj);
		result= dagNode.partialPathName();
	}
	else
	{
		MFnDependencyNode depNode(obj);
		result= depNode.name();
	}

	return result;
}


MStatus	DxMPxCommand::doIt ( const MArgList& args )	
{
	if( ArgCount != -1 
	&&  ArgCount != (int)args.length())
	{
		displayError( MString("Use '-help' argument for usage info.") );
		return MStatus::kFailure;
	}

	MString Arg0;
	args.get(0, Arg0);

	if(Arg0 == "-help")
	{
		setResult(  MString("Usage: ") + MString( Help.GetString() )  );
		return MStatus::kSuccess;
	}

	return Run(args);
}



MStatus	DXMListGraphsCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	for( POSITION position= DXMUnitTests::Graphs.GetHeadPosition();
		position != NULL;
		DXMUnitTests::Graphs.GetNext(position))
	{
		MString name= DXMUnitTests::Graphs.GetKeyAt(position).GetString();
		result.append( name );
		
	}
		
	setResult(result);

	return MStatus::kSuccess;
}





MStatus	DXMGraphListAdaptersCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}


	for( DXMGraph::DXMAdapterIterator adapterItr= graph->GetAdapterIterator();
		!adapterItr.Done();
		adapterItr.Next())
	{
		DXMGraphAdapter* adapter= NULL;
		adapterItr.GetValue(adapter);

		result.append( MString( adapter->Signature() ) );
	}

	setResult(result);

	return MStatus::kSuccess;
}

MStatus	DXMGraphHasAdapterCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMGraphAdapter* adapter= graph->FindAdapter(Arg1.asChar());


	setResult(adapter != NULL);

	return MStatus::kSuccess;
}

MStatus	DXMGraphListNodesCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	for( DXMGraph::DXMNodeIterator nodeItr= graph->GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue(node);


		MObject obj= node->GetSite();
		MString name= DXMUnitTests::GetObjectName(obj);
		

		result.append(name);
	}

	setResult(result);

	return MStatus::kSuccess;
}

MStatus	DXMGraphHasInvalidNodesCommand::Run ( const MArgList& args )	
{
	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	for( DXMGraph::DXMNodeIterator nodeItr= graph->GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue(node);

		if(node->GetSite().isNull())
		{
			setResult(true);
			return MStatus::kSuccess;
		}
	}

	setResult(false);

	return MStatus::kSuccess;
}

MStatus	DXMGraphHasNodeCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	MObject obj= DXMUnitTests::GetObject(Arg1);
	DXMNode* node= graph->FindNode(obj);


	setResult(node != NULL);

	return MStatus::kSuccess;
}

MStatus	DXMNodeListAdaptersCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}


	for( DXMNode::DXMAdapterIterator adapterItr= node->GetAdapterIterator();
		!adapterItr.Done();
		adapterItr.Next())
	{
		DXMNodeAdapter* adapter= NULL;
		adapterItr.GetValue(adapter);

		result.append( MString( adapter->Signature() ) );
	}

	setResult(result);

	return MStatus::kSuccess;

}

MStatus	DXMNodeHasAdapterCommand::Run ( const MArgList& args )	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	DXMNodeAdapter* adapter= node->FindAdapter(Arg2.asChar());

	setResult(adapter != NULL);

	return MStatus::kSuccess;

}

MStatus	DXMNodeAdapterGetOwnerCommand::Run ( const MArgList& args)	
{
	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	DXMNodeAdapter* nodeAdapter= DXMUnitTests::GetNodeAdapter(node, Arg2 );
	if(!nodeAdapter)
	{
		displayError(MString("Failed to find nodeAdapter"));
		return MStatus::kFailure;
	}

	setResult( MString( nodeAdapter->OwnerSignature() ) );

	return MStatus::kSuccess;
}

MStatus	DXMNodeDagIsExplicitlyInvisibleCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter( DXMNodeDagAdapter::MySignature ) );
	if(!nodeDagAdapter)
	{
		displayError(MString("Failed to find nodeDagAdapter"));
		return MStatus::kFailure;
	}

	setResult(nodeDagAdapter->IsExplicitlyInvisible());

	return MStatus::kSuccess;
}

MStatus	DXMNodeMeshIsAnyRouteVisibleCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	DXMNodeMeshAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( node->FindAdapter( DXMNodeMeshAdapter::MySignature ) );
	if(!nodeDagAdapter)
	{
		displayError(MString("Failed to find nodeMeshAdapter"));
		return MStatus::kFailure;
	}

	setResult(nodeDagAdapter->IsAnyRouteVisible());

	return MStatus::kSuccess;
}

MStatus	DXMNodeDagGetLocalMatrixCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}


	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter( DXMNodeDagAdapter::MySignature ) );
	if(!nodeDagAdapter)
	{
		displayError(MString("node is not a DAGNode so no LocalMatrix exists"));
		return MStatus::kFailure;
	}


	MDoubleArray matrixAsDblArray;

	D3DXMATRIX d3dmatrix= nodeDagAdapter->GetLocalMatrix();

	matrixAsDblArray.setLength(16);
	for(UINT i= 0; i < 16; i++)
	{
		matrixAsDblArray[i]= ((FLOAT*)d3dmatrix)[i];
	}
	setResult(matrixAsDblArray);

	return MStatus::kSuccess;

}

MStatus	DXMGraphListRoutesCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	for( DXMGraph::DXMNodeIterator nodeItr= graph->GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue(node);

		DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter(DXMNodeDagAdapter::MySignature ) );
		if(!nodeDagAdapter)
			continue;		
		

		for( DXMNodeDagAdapter::DXMRouteIterator routeItr= nodeDagAdapter->GetRouteIterator();
			!routeItr.Done();
			routeItr.Next())
		{
			DXMRoute* route= NULL;
			routeItr.GetValue(route);

			MDagPath path= route->GetPath();
			result.append( DXMUnitTests::GetPathName( path ) );
		}

	}

	setResult(result);

	return MStatus::kSuccess;

}

MStatus	DXMGraphHasInvalidRoutesCommand::Run ( const MArgList& args)	
{
	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	for( DXMGraph::DXMNodeIterator nodeItr= graph->GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue(node);

		DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter(DXMNodeDagAdapter::MySignature ) );
		if(!nodeDagAdapter)
			continue;		
		
		for( DXMNodeDagAdapter::DXMRouteIterator routeItr= nodeDagAdapter->GetRouteIterator();
			!routeItr.Done();
			routeItr.Next())
		{
			DXMRoute* route= NULL;
			routeItr.GetValue(route);

			if(!route->GetPath().isValid())
			{
				setResult(true);
				return MStatus::kSuccess;
			}
		}
	}

	setResult(false);

	return MStatus::kSuccess;

}

MStatus	DXMNodeDagListRoutesCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}


	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( DXMUnitTests::GetNodeAdapter(node, MString( DXMNodeDagAdapter::MySignature.GetString() )) );
	if(!nodeDagAdapter)
	{
		displayError(MString("Failed to find nodeDagAdapter"));
		return MStatus::kFailure;
	}

	for( DXMNodeDagAdapter::DXMRouteIterator routeItr= nodeDagAdapter->GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);

		MDagPath path= route->GetPath();
		result.append( DXMUnitTests::GetPathName( path ) );
	}

	setResult(result);

	return MStatus::kSuccess;
}

MStatus	DXMRouteGetMemberCountCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	setResult( (int)( route->Frame->NumMembers() ) );
	return MStatus::kSuccess;
}

MStatus	DXMNodeMeshIsMemberOfRouteCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	DXMNodeMeshAdapter* nodeMeshAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( node->FindAdapter( DXMNodeMeshAdapter::MySignature.GetString() ) );
	if(!nodeMeshAdapter)
	{
		displayError(MString("Failed to find nodeMeshAdapter"));
		return MStatus::kFailure;
	}


	MString Arg2;
	args.get(2, Arg2);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg2);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	if(!route->Frame || !nodeMeshAdapter->Mesh)
	{
		setResult(false);
		return MStatus::kSuccess;
	}

	UINT meshIndex;
	if(DXCC_SUCCEEDED(route->Frame->FindMemberByPointer( nodeMeshAdapter->Mesh, &meshIndex)))
	{
		setResult(true);
		return MStatus::kSuccess;
	}

	setResult(false);
	return MStatus::kSuccess;
}

MStatus	DXMNodeDagHasRouteCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( DXMUnitTests::GetNodeAdapter(node, MString( DXMNodeDagAdapter::MySignature.GetString() )) );
	if(!nodeDagAdapter)
	{
		displayError(MString("Failed to find nodeDagAdapter"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	MDagPath path = DXMUnitTests::GetPath(Arg2);
	if(!path.isValid())
	{
		displayError(MString("could not convert arg2 to dagpath"));
		return MStatus::kFailure;
	}

	DXMRoute* route= nodeDagAdapter->FindRoute(path);
	
	setResult( route != NULL );
	return MStatus::kSuccess;
}

MStatus	DXMRouteGetParentCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}


	DXMRoute* parentRoute= route->GetParentRoute();
	if(!parentRoute)
	{
		//empty result;
		return MStatus::kSuccess;
	}

	MDagPath path= route->GetPath();
	setResult( DXMUnitTests::GetPathName( path ) );

	return MStatus::kSuccess;
}

MStatus	DXMRouteGetNodeCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	setResult( DXMUnitTests::GetObjectName( route->GetNodeDagAdapter()->GetNode()->GetSite() ) );

	return MStatus::kSuccess;
}

MStatus	DXMRouteListChildrenCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	for( DXMRoute::DXMRouteIterator routeItr= route->GetChildRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* child= NULL;
		routeItr.GetValue(child);
		
		MDagPath childPath= child->GetPath();
		result.append( DXMUnitTests::GetPathName(childPath));
	}

	setResult( result );
	return MStatus::kSuccess;
}


MStatus	DXMRouteHasChildCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	DXMRoute* child= route->FindChild(DXMUnitTests::GetPath(Arg2));

	setResult( child != NULL );

	return MStatus::kSuccess;
}


MStatus	DXMRouteIsFrameParentingEqualCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}
	DXMRoute* parentRoute= route->GetParentRoute();

	LPDXCCFRAME pParentFrame= NULL;
	route->Frame->GetParent(&pParentFrame);
	if(!pParentFrame)
	{
		displayError(MString("Failed to find parent frame from route->Frame"));
		setResult(false);
	}
	else if(!parentRoute && (pParentFrame == g_PreviewPipeline.AccessRoot()))
	{
		setResult(true);
	}
	else if(pParentFrame == parentRoute->Frame)
	{
		setResult(true);
	}
	else
		setResult(false);

	DXCC_RELEASE(pParentFrame);

	return MStatus::kSuccess;
}

MStatus	DXMGraphHasRouteCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);

	setResult( route != NULL );

	return MStatus::kSuccess;
}

MStatus	DXMRouteIsVisibleCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	setResult(route->IsVisible());

	return MStatus::kSuccess;
}

MStatus	DXMRouteGetLocalMatrixCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	MDoubleArray matrixAsDblArray;

	D3DXMATRIX d3dmatrix= *( route->Frame->GetLocalMatrix() );

	matrixAsDblArray.setLength(16);
	for(UINT i= 0; i < 16; i++)
	{
		matrixAsDblArray[i]= ((FLOAT*)d3dmatrix)[i];
	}
	setResult(matrixAsDblArray);

	return MStatus::kSuccess;
}


MStatus	DXMRouteGetWorldMatrixCommand::Run ( const MArgList& args)	
{
	MStringArray result;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMRoute* route= DXMUnitTests::GetRoute(graph, Arg1);
	if(!route)
	{
		displayError(MString("Failed to find route"));
		return MStatus::kFailure;
	}

	MDoubleArray matrixAsDblArray;

	D3DXMATRIX d3dmatrix= *( route->Frame->GetWorldMatrix() );

	matrixAsDblArray.setLength(16);
	for(UINT i= 0; i < 16; i++)
	{
		matrixAsDblArray[i]= ((FLOAT*)d3dmatrix)[i];
	}
	setResult(matrixAsDblArray);

	return MStatus::kSuccess;
}


MStatus	DXMGraphAdapterIsInterestedCommand::Run ( const MArgList& args)	
{
	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMGraphAdapter* graphAdapter= DXMUnitTests::GetGraphAdapter(graph, Arg1);
	if(!graphAdapter)
	{
		displayError(MString("Failed to find graphAdapter"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	MObject obj= DXMUnitTests::GetObject(Arg2);
	if(obj.isNull())
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	
	setResult( graphAdapter->IsInterested(obj) );

	return MStatus::kSuccess;
}

MStatus	DXMNodeCountAdaptersOwnedByCommand::Run ( const MArgList& args)	
{
	int result= 0;

	MString Arg0;
	args.get(0, Arg0);

	DXMGraph* graph= DXMUnitTests::GetGraph(Arg0);
	if(!graph)
	{
		displayError(MString("Failed to find graph"));
		return MStatus::kFailure;
	}

	MString Arg1;
	args.get(1, Arg1);

	DXMNode* node= DXMUnitTests::GetNode(graph, Arg1);
	if(!node)
	{
		displayError(MString("Failed to find node"));
		return MStatus::kFailure;
	}

	MString Arg2;
	args.get(2, Arg2);

	for(DXMNode::DXMAdapterIterator nodeAdapterItr= node->GetAdapterIterator();
		!nodeAdapterItr.Done();
		nodeAdapterItr.Next())
	{
		DXMNodeAdapter* nodeAdapter= NULL;
		nodeAdapterItr.GetValue(nodeAdapter);

		if( nodeAdapter->OwnerSignature() == Arg2.asChar() )
			result++;
	}

	setResult( result );

	return MStatus::kSuccess;
}