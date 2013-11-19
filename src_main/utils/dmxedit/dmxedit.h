//=============================================================================
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// $Header: $
// $NoKeywords: $
//
// Converts from any one DMX file format to another
// Can also output SMD or a QCI header from DMX input
//
//=============================================================================

#ifndef DMXEDIT_H
#define DMXEDIT_H


// Valve includes
#include "movieobjects/dmemesh.h"
#include "tier1/utlstring.h"
#include "tier1/utlvector.h"
#include "tier1/UtlStringMap.h"

// Lua includes
#include <lua.h>


class CDmxEditProxy;

//=============================================================================
// CDmxEdit declaration
//=============================================================================
class CDmxEdit
{
public:
	class delta : public CUtlString
	{
	public:
		delta() {};
		delta( const char *pString ) : CUtlString( pString ) {}
		delta( const CUtlString &string ) : CUtlString( string ) {}
	};

	class CFalloffType
	{
	public:
		CFalloffType( CDmeMesh::Falloff_t falloffType )
		: m_falloffType( falloffType )
		{
		}

		CDmeMesh::Falloff_t StringToFalloff( const char *pFalloffTypeString )
		{
			if ( !Q_strnicmp( pFalloffTypeString, "L", 1 ) )
				return CDmeMesh::LINEAR;

			if ( !Q_strnicmp( pFalloffTypeString, "ST", 2 ) )
				return CDmeMesh::STRAIGHT;

			if ( !Q_strnicmp( pFalloffTypeString, "B", 1 ) )
				return CDmeMesh::BELL;

			if ( !Q_strnicmp( pFalloffTypeString, "SM", 2 ) )
				return CDmeMesh::SMOOTH;

			if ( !Q_strnicmp( pFalloffTypeString, "SP", 2 ) )
				return CDmeMesh::SPIKE;

			if ( !Q_strnicmp( pFalloffTypeString, "D", 1 ) )
				return CDmeMesh::DOME;

			Msg( "// ERROR: Can't Figure Out Which Falloff Type Is Meant By \"%s\", Assuming STRAIGHT\n", pFalloffTypeString );

			return CDmeMesh::STRAIGHT;
		}

		CFalloffType( const char *pFalloffTypeString )
		: m_falloffType( StringToFalloff( pFalloffTypeString ) )
		{
		}

		CDmeMesh::Falloff_t operator()() const { return m_falloffType; }

		operator char *() const
		{
			switch ( m_falloffType )
			{
			case CDmeMesh::LINEAR:
				return "STRAIGHT";
			case CDmeMesh::BELL:
				return "BELL";
			case CDmeMesh::SPIKE:
				return "SPIKE";
			case CDmeMesh::DOME:
				return "DOME";
			default:
				break;
			}

			return "STRAIGHT";
		}

	protected:
		const CDmeMesh::Falloff_t m_falloffType;
	};

	static const CFalloffType STRAIGHT;
	static const CFalloffType LINEAR;
	static const CFalloffType BELL;
	static const CFalloffType SMOOTH;
	static const CFalloffType SPIKE;
	static const CFalloffType DOME;

	class CSelectOp
	{
	public:
		enum SelectOp_t
		{
			kAdd,
			kSubtract,
			kToggle,
			kIntersect,
			kReplace
		};

		CSelectOp( SelectOp_t selectOp )
		: m_selectOp( selectOp )
		{
		}

		SelectOp_t StringToSelectOp( const char *pSelectOpString )
		{
			if ( !Q_strnicmp( pSelectOpString, "A", 1 ) )
				return kAdd;

			if ( !Q_strnicmp( pSelectOpString, "S", 1 ) )
				return kSubtract;

			if ( !Q_strnicmp( pSelectOpString, "T", 1 ) )
				return kToggle;

			if ( !Q_strnicmp( pSelectOpString, "I", 1 ) )
				return kIntersect;

			if ( !Q_strnicmp( pSelectOpString, "R", 1 ) )
				return kReplace;

			Msg( "// ERROR: Can't Figure Out Which Select Operation Type Is Meant By \"%s\", Assuming REPLACE\n", pSelectOpString );

			return kReplace;
		}

		CSelectOp( const char *pSelectOp )
		: m_selectOp( StringToSelectOp( pSelectOp ) )
		{
		}

		SelectOp_t operator()() const { return m_selectOp; }

		operator char *() const
		{
			switch ( m_selectOp )
			{
			case kAdd:
				return "ADD";
			case kSubtract:
				return "SUBTRACT";
			case kToggle:
				return "TOGGLE";
			case kIntersect:
				return "INTERSECT";
			default:
				break;
			}

			return "REPLACE";
		}

	protected:
		const SelectOp_t m_selectOp;
	};

	static const CSelectOp ADD;
	static const CSelectOp SUBTRACT;
	static const CSelectOp TOGGLE;
	static const CSelectOp INTERSECT;
	static const CSelectOp REPLACE;

	class CSelectType
	{
	public:
		enum Select_t
		{
			kNone,
			kAll,
			kDelta
		};

		CSelectType( Select_t selectType )
		: m_selectType( selectType )
		{
		}

		Select_t SelectTypeStringToType( const char *pSelectString )
		{
			if ( !Q_stricmp( pSelectString, "NONE" ) )
				return kNone;

			if ( !Q_stricmp( pSelectString, "ALL" ) )
				return kAll;

			return kDelta;
		}

		CSelectType( const char *pSelectTypeString )
		: m_selectType( SelectTypeStringToType( pSelectTypeString ) )
		{
		}

		Select_t operator()() const { return m_selectType; }

		operator char *() const
		{
			switch ( m_selectType )
			{
			case kNone:
				return "NONE";
			case kAll:
				return "ALL";
			default:
				break;
			}

			return "DELTA";
		}

	protected:
		const Select_t m_selectType;
	};

	static const CSelectType ALL;
	static const CSelectType NONE;

	//=============================================================================
	//
	//=============================================================================
	class CObjType
	{
	public:
		enum Obj_t
		{
			kAbsolute,
			kRelative
		};

		CObjType( Obj_t objType )
		: m_objType( objType )
		{
		}

		CObjType &operator=( const CObjType &rhs )
		{
			m_objType = rhs.m_objType;
			return *this;
		}

		Obj_t ObjTypeStringToType( const char *pObjString )
		{
			if ( pObjString && ( *pObjString == 'r' || *pObjString == 'R' ) )
				return kRelative;

			return kAbsolute;
		}

		CObjType( const char *pObjTypeString )
		: m_objType( ObjTypeStringToType( pObjTypeString ) )
		{
		}

		Obj_t operator()() const { return m_objType; }

		operator char *() const
		{
			switch ( m_objType )
			{
			case kRelative:
				return "RELATIVE";
			default:
				break;
			}

			return "ABSOLUTE";
		}



	protected:
		Obj_t m_objType;
	};

	static const CObjType ABSOLUTE;
	static const CObjType RELATIVE;


	//=============================================================================
	//
	//=============================================================================
	class CDistanceType
	{
	public:
		CDistanceType( CDmeMesh::Distance_t distanceType )
		: m_distanceType( distanceType )
		{
		}

		CDmeMesh::Distance_t DistanceTypeStringToType( const char *pDistanceTypeString )
		{
			if ( pDistanceTypeString && ( *pDistanceTypeString == 'a' || *pDistanceTypeString == 'A' ) )
				return CDmeMesh::DIST_ABSOLUTE;

			if ( pDistanceTypeString && ( *pDistanceTypeString == 'r' || *pDistanceTypeString == 'R' ) )
				return CDmeMesh::DIST_RELATIVE;

			return CDmeMesh::DIST_DEFAULT;
		}

		CDistanceType( const char *pDistanceTypeString )
		: m_distanceType( DistanceTypeStringToType( pDistanceTypeString ) )
		{
		}

		CDmeMesh::Distance_t operator()() const { return m_distanceType; }

		operator char *() const
		{
			switch ( m_distanceType )
			{
			case CDmeMesh::DIST_ABSOLUTE:
				return "ABSOLUTE";
			case CDmeMesh::DIST_RELATIVE:
				return "RELATIVE";
			default:
				break;
			}

			return "DEFAULT";
		}

	protected:
		CDmeMesh::Distance_t m_distanceType;
	};

	static const CDistanceType DIST_ABSOLUTE;
	static const CDistanceType DIST_RELATIVE;
	static const CDistanceType DIST_DEFAULT;

	class CAxisType
	{
	public:
		enum Axis_t
		{
			kXAxis,
			kYAxis,
			kZAxis
		};

		CAxisType( Axis_t axisType )
		: m_axisType( axisType )
		{
		}

		Axis_t AxisTypeStringToType( const char *pAxisString )
		{
			if ( pAxisString && ( *pAxisString == 'y' || *pAxisString == 'Y' ) )
				return kYAxis;

			if ( pAxisString && ( *pAxisString == 'z' || *pAxisString == 'Z' ) )
				return kZAxis;

			return kXAxis;
		}

		CAxisType( const char *pAxisTypeString )
		: m_axisType( AxisTypeStringToType( pAxisTypeString ) )
		{
		}

		Axis_t operator()() const { return m_axisType; }

		operator char *() const
		{
			switch ( m_axisType )
			{
			case kYAxis:
				return "YAXIS";
			case kZAxis:
				return "ZAXIS";
			default:
				break;
			}

			return "XAXIS";
		}

	protected:
		const Axis_t m_axisType;
	};

	static const CAxisType XAXIS;
	static const CAxisType YAXIS;
	static const CAxisType ZAXIS;

	class CHalfType
	{
	public:
		enum Half_t
		{
			kLeft,
			kRight
		};

		CHalfType( Half_t halfType )
		: m_halfType( halfType )
		{
		}

		Half_t HalfTypeStringToType( const char *pHalfString )
		{
			if ( pHalfString && ( *pHalfString == 'l' || *pHalfString == 'L' ) )
				return kLeft;

			if ( pHalfString && ( *pHalfString == 'r' || *pHalfString == 'R' ) )
				return kRight;

			return kLeft;
		}

		CHalfType( const char *pHalfTypeString )
		: m_halfType( HalfTypeStringToType( pHalfTypeString ) )
		{
		}

		Half_t operator()() const { return m_halfType; }

		operator char *() const
		{
			switch ( m_halfType )
			{
			case kLeft:
				return "LEFT";
			case kRight:
				return "RIGHT";
			default:
				break;
			}

			return "LEFT";
		}

	protected:
		const Half_t m_halfType;
	};

	static const CHalfType LEFT;
	static const CHalfType RIGHT;

	CDmxEdit();

	virtual ~CDmxEdit() {};

	bool Load( const char *pFilename, const CObjType &loadType = ABSOLUTE );

	bool Import( const char *pFilename, const char *pParentName = NULL );

	operator bool() const { return m_pMesh != NULL; }

	void DoIt();

	void ListDeltas();

	void Unload();

	bool ImportComboRules( const char *pFilename, bool bOverwrite = true, bool bPurgeDeltas = true );

	bool ResetState();

	bool SetState( const char *pDeltaName );

	bool Select( const char *selectString, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool Select( const CSelectType &selectType, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool Select( const CSelectOp &selectOp, const char *pSelectTypeString, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool Select( const CSelectOp &selectOp, const CSelectType &selectType, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool SelectHalf( const CSelectOp &selectOp, const CHalfType &halfType, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool SelectHalf( const CHalfType &halfType, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool GrowSelection( int nSize = 1 );

	bool ShrinkSelection( int nSize = 1 );

	bool Add( const CDmxEditProxy &e, float weight = 1.0f, float featherDistance = 0.0f, const CFalloffType &falloffType = STRAIGHT, const CDistanceType &distanceType = DIST_DEFAULT );

	bool Add( const char *pDeltaName, float weight = 1.0f, float featherDistance = 0.0f, const CFalloffType &falloffType = STRAIGHT, const CDistanceType &distanceType = DIST_DEFAULT );

	bool Interp( const CDmxEditProxy &e, float weight = 1.0f, float featherDistance = 0.0f, const CFalloffType &falloffType = STRAIGHT, const CDistanceType &distanceType = DIST_DEFAULT );

	bool Interp( const char *pDeltaName, float weight = 1.0f, float featherDistance = 0.0f, const CFalloffType &falloffType = STRAIGHT, const CDistanceType &distanceType = DIST_DEFAULT );

	CUtlString SaveDelta( const char *pDeltaName );

	bool DeleteDelta( const delta &d );

	bool Save( const char *pFilename, const CObjType &saveType = ABSOLUTE, const char *pDeltaName = NULL );

	bool Save() { return Save( m_filename ); }

	bool Merge( const char *pInFilename, const char *pOutFilename );

	bool RemapMaterial( int nMaterialIndex, const char *pNewMaterialName );

	bool RemapMaterial( const char *pOldMaterialName, const char *pNewMaterialName );

	bool RemoveFacesWithMaterial( const char *pMaterialName );

	bool RemoveFacesWithMoreThanNVerts( int nVertexCount );

	bool Mirror( CAxisType = XAXIS );

	bool ComputeNormals();

	bool CreateDeltasFromPresets( const char *pPresetFilename, bool bDeleteNonPresetDeltas = true, const CUtlVector< CUtlString > *pPurgeAllButThese = NULL, const char *pExpressionFilename = NULL );

	bool CachePreset( const char *pPresetFilename, const char *pExpressionFilename = NULL );

	bool ClearPresetCache();

	bool CreateDeltasFromCachedPresets( bool bDeleteNonPresetDeltas = true, const CUtlVector< CUtlString > *pPurgeAllButThese = NULL ) const;

	bool CreateExpressionFileFromPresets( const char *pPresetFilename, const char *pExpressionFilename );

	bool CreateExpressionFilesFromCachedPresets() const;

	bool ComputeWrinkles();

	bool ComputeWrinkle( const char *pDeltaName, float scale );

	bool Scale( float sx, float sy, float sz );

	bool SetDistanceType( const CDistanceType &distanceType );

	bool Translate(
		Vector t,
		float featherDistance = 0.0f,
		const CFalloffType &falloffType = STRAIGHT,
		const CDistanceType &distanceType = DIST_DEFAULT,
		CDmeMesh *pPassedMesh = NULL,
		CDmeVertexData *pPassedBase = NULL,
		CDmeSingleIndexedComponent *pPassedSelection = NULL );

	bool Translate( Vector t, float featherDistance, const CFalloffType &falloffType = STRAIGHT )
	{
		return Translate( t, featherDistance, falloffType, m_distanceType );
	}

	bool FixPresetFile( const char *pPresetFilename );

	bool ErrorState() {
		return m_errorState;
	}

	void Error( const tchar *pMsgFormat, ... );

protected:
	CUtlString m_filename;
	CDmElement *m_pRoot;
	CDmeMesh *m_pMesh;
	CDmeSingleIndexedComponent *m_pCurrentSelection;
	bool m_errorState;
	CDistanceType m_distanceType;

	CUtlStringMap< CUtlString > m_presetCache;

	bool Select( CDmeVertexDeltaData *pDelta, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	bool Select( const CSelectOp &selectOp, CDmeSingleIndexedComponent *pOriginal, const CDmeSingleIndexedComponent *pNew );

	bool Select( const CSelectOp &selectOp, CDmeVertexDeltaData *pDelta, CDmeSingleIndexedComponent *pPassedSelection = NULL, CDmeMesh *pPassedMesh = NULL );

	void ImportCombinationControls( CDmeCombinationOperator *pSrcComboOp, CDmeCombinationOperator *pDstComboOp, bool bOverwrite );

	void ImportDominationRules( CDmeCombinationOperator *pDestComboOp, CDmeCombinationOperator *pSrcComboOp, bool bOverwrite );

	CDmeMesh *GetMesh( CDmeMesh *pPassedMesh )
	{
		return pPassedMesh ? pPassedMesh : m_pMesh;
	}

	void SetErrorState()
	{
		m_errorState = true;
	}

	void ResetErrorState()
	{
		m_errorState = false;
	}

	CDmeSingleIndexedComponent *GetSelection( CDmeSingleIndexedComponent *pPassedSelection )
	{
		return pPassedSelection ? pPassedSelection : m_pCurrentSelection;
	}

	CDmeVertexDeltaData *FindDeltaState( const char *pDeltaName, const CDmeMesh *pPassedMesh = NULL ) const
	{
		const CDmeMesh *pMesh = pPassedMesh ? pPassedMesh : m_pMesh;
		if ( !pMesh )
			return NULL;

		return pMesh->FindDeltaState( pDeltaName );
	}
};


//=============================================================================
//
//=============================================================================
class CDmxEditLua
{
public:
	CDmxEditLua();

	bool DoIt( const char *pFilename );

	void SetVar( const CUtlString &var, const CUtlString &val );

	void SetGame( const CUtlString &game );

	typedef int ( * LuaFunc_t ) ( lua_State * );

	struct LuaFunc_s
	{
		const char *m_pFuncName;
		LuaFunc_t m_pFunc;
		const char *m_pFuncPrototype;
		const char *m_pFuncDesc;
	};

	static int FunctionCount();

	static const LuaFunc_s *GetFunctionList();

protected:
	static LuaFunc_s s_luaFuncs[];

	static int Load( lua_State *pLuaState );

	static int Import( lua_State *pLuaState );

	static int Save( lua_State *pLuaState );

	static int ListDeltas( lua_State *pLuaState );

	static int ImportComboRules( lua_State *pLuaState );

	static int ResetState( lua_State *pLuaState );

	static int SetState( lua_State *pLuaState );

	static int Select( lua_State *pLuaState );

	static int SelectHalf( lua_State *pLuaState );

	static int GrowSelection( lua_State *pLuaState );

	static int ShrinkSelection( lua_State *pLuaState );

	static int Add( lua_State *pLuaState );

	static int Interp( lua_State *pLuaState );

	static int SaveDelta( lua_State *pLuaState );

	static int DeleteDelta( lua_State *pLuaState );

	static int Merge( lua_State *pLuaState );

	static int RemapMaterial( lua_State *pLuaState );

	static int RemoveFacesWithMaterial( lua_State *pLuaState );

	static int RemoveFacesWithMoreThanNVerts( lua_State *pLuaState );

	static int Mirror( lua_State *pLuaState );

	static int ComputeNormals( lua_State *pLuaState );

	static int CreateDeltasFromPresets( lua_State *pLuaState );

	static int CachePreset( lua_State *pLuaState );

	static int ClearPresetCache( lua_State *pLuaState );

	static int CreateDeltasFromCachedPresets( lua_State *pLuaState );

	static int CreateExpressionFileFromPresets( lua_State *pLuaState );

	static int CreateExpressionFilesFromCachedPresets( lua_State *pLuaState );

	static int ComputeWrinkles( lua_State *pLuaState );

	static int ComputeWrinkle( lua_State *pLuaState );

	static int Scale( lua_State *pLuaState );

	static int Translate( lua_State *pLuaState );

	static int SetDistanceType( lua_State *pLusState );

	static int FixPresetFile( lua_State *pLuaState );

	static int FileExists( lua_State *pLuaState );

protected:
	lua_State *m_pLuaState;
	// TODO: Store this into the lua state somehow...
	static CDmxEdit m_dmxEdit;
};



//=============================================================================
//
//=============================================================================
class CDmxEditProxy
{
public:
	CDmxEditProxy( CDmxEdit &dmxEdit )
	: m_dmxEdit( dmxEdit )
	{}

	CDmxEditProxy &operator=( const char *pFilename ) { m_dmxEdit.Load( pFilename ); return *this; }

	operator bool() const { m_dmxEdit; }

protected:
	CDmxEdit &m_dmxEdit;
};


#endif // DMXEDIT_H