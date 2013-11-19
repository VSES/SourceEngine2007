//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "dmserializerxml.h"
#include "datamodel/idatamodel.h"
#include "datamodel.h"
#include "datamodel/dmattributevar.h"
#include "datamodel/dmelement.h"
#include "dmattributeinternal.h"
#include "dmelementdictionary.h"
#include "tier1/uniqueid.h"
#include "tier1/utlbuffer.h"
#include "tier1/utlrbtree.h"
#include "tier1/utlstack.h"
#include "parsifal/parsifal.h"
#include "DmElementFramework.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CUtlBuffer;
class CBaseSceneObject;


//-----------------------------------------------------------------------------
// DMX Header block
//-----------------------------------------------------------------------------
#define DMX_FILE_STARTING_TOKEN "DMXFile"
#define DMX_FILE_ENDING_TOKEN "DMXFile"


//-----------------------------------------------------------------------------
// Character conversion rules
//-----------------------------------------------------------------------------
BEGIN_CHAR_CONVERSION( s_XMLCharConversion, "", '&' )
	{ '&',  "amp;" },
	{ '<',  "lt;" },
	{ '>',  "gt;" },
	{ '\"', "quot;" },
	{ '\'', "apos;" },
END_CHAR_CONVERSION( s_XMLCharConversion, "", '&' )

// Just delimiters, no type conversion
class CXMLInputStringConversion : public CUtlCharConversion
{
public:
	CXMLInputStringConversion( char nEscapeChar, const char *pDelimiter, int nCount, ConversionArray_t *pArray ) :
		CUtlCharConversion( '\0', pDelimiter, 0, NULL )
	{
	}

	// Finds a conversion for the passed-in string, returns length
	virtual char FindConversion( const char *pString, int *pLength )
	{
		*pLength = 0;
		return '\0';
	}
};


//-----------------------------------------------------------------------------
// Base serialization class 
//-----------------------------------------------------------------------------
class CXMLSerializer : public IDmSerializer
{
public:
	CXMLSerializer( bool bFlatMode ) : m_bFlatMode( bFlatMode ) {}

	// Inherited from IDMSerializer
	virtual const char *GetName() const { return m_bFlatMode ? "xml_flat" : "xml"; }
	virtual const char *GetDescription() const { return m_bFlatMode ? "XML (flat)" : "XML"; }
	virtual bool StoresVersionInFile() const { return true; }
	virtual bool IsBinaryFormat() const { return false; }
	virtual int GetCurrentVersion() const { return 1; }
	virtual bool Serialize( CUtlBuffer &buf, CDmElement *pRoot );
	virtual bool Unserialize( CUtlBuffer &buf, const char *pEncodingName, int nEncodingVersion,
							  const char *pSourceFormatName, int nSourceFormatVersion,
							  DmFileId_t fileid, DmConflictResolution_t idConflictResolution, CDmElement **ppRoot );

private:
	void SerializeElementReference( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement );
	void SerializeElementArrayAttribute( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, const char *pName, CDmAttribute *pAttribute );
	void SerializeElementAttribute( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, const char *pName, CDmAttribute *pAttribute );
	void SerializeArrayAttribute( CUtlBuffer& buf, CDmAttribute *pAttribute );
	bool SerializeAttributes( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement );
	bool SaveElement( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement, bool bWriteDelimiters = true );
	bool UnserializeElements( CUtlBuffer &buf, const char *pFormatName, DmFileId_t fileid, DmConflictResolution_t idConflictResolution, CDmElement **ppRoot );

	bool m_bFlatMode;
};


//-----------------------------------------------------------------------------
// Singleton instance
//-----------------------------------------------------------------------------
static CXMLSerializer s_XMLSerializer( false );
static CXMLSerializer s_XMLSerializerFlat( true );

void InstallXMLSerializer( IDataModel *pFactory )
{
	pFactory->AddSerializer( &s_XMLSerializer );
	pFactory->AddSerializer( &s_XMLSerializerFlat );
}


//-----------------------------------------------------------------------------
// Serializes a single element attribute
//-----------------------------------------------------------------------------
void CXMLSerializer::SerializeElementReference( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement )
{
	if ( dict.ShouldInlineElement( pElement ) )
	{
		SaveElement( buf, dict, pElement );
	}
	else
	{
		const char *pAttributeType = AttributeTypeName( AT_ELEMENT );
		buf.Printf( "<%s>", pAttributeType );
		if ( pElement )
		{
			::Serialize( buf, pElement->GetId() );
		}
		buf.Printf( "</%s>\n", pAttributeType );
	}
}

	
//-----------------------------------------------------------------------------
// Serializes a single element attribute
//-----------------------------------------------------------------------------
void CXMLSerializer::SerializeElementAttribute( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, const char *pName, CDmAttribute *pAttribute )
{
	CDmElement *pElement = pAttribute->GetValueElement<CDmElement>();
	if ( dict.ShouldInlineElement( pElement ) )
	{
		char idBuf[256];
		const DmObjectId_t &id = pElement->GetId();
		UniqueIdToString( id, idBuf, sizeof(idBuf) );

		buf.Printf( "<%s type=\"%s\" name=\"%s\" id=\"%s\">\n", 
			pName, pElement->GetTypeString(), pElement->GetName(), idBuf );
		SaveElement( buf, dict, pElement, false );
		buf.Printf( "</%s>\n", pName );
	}
	else
	{
		const char *pAttributeType = AttributeTypeName( AT_ELEMENT );
		buf.Printf( "<%s type=\"%s\">", pName, pAttributeType );
		if ( pElement )
		{
			::Serialize( buf, pElement->GetId() );
		}
		buf.Printf( "</%s>\n", pName );
	}
}


//-----------------------------------------------------------------------------
// Serializes an array of element attribute
//-----------------------------------------------------------------------------
void CXMLSerializer::SerializeElementArrayAttribute( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, const char *pName, CDmAttribute *pAttribute )
{
	CDmrElementArray<> array( pAttribute );

	const char *pAttributeType = AttributeTypeName( AT_ELEMENT_ARRAY );
	buf.Printf( "<%s type=\"%s\">\n", pName, pAttributeType );
	buf.PushTab();

	int nCount = array.Count();
	for ( int i = 0; i < nCount; ++i )
	{
		CDmElement *pElement = array[i];
		SerializeElementReference( buf, dict, pElement );
	}

	buf.PopTab();
	buf.Printf( "</%s>\n", pName );
}


//-----------------------------------------------------------------------------
// Serializes array attributes
//-----------------------------------------------------------------------------
void CXMLSerializer::SerializeArrayAttribute( CUtlBuffer& buf, CDmAttribute *pAttribute )
{
	CDmrGenericArray array( pAttribute );
	int nCount = array.Count();

	for ( int i = 0; i < nCount; ++i )
	{
		buf.PutString( "<value>" );
		buf.PushTab();
		array.GetAttribute()->SerializeElement( i, buf );
		buf.PopTab();
		buf.PutString( "</value>\n" );
	}
}


//-----------------------------------------------------------------------------
// Serializes a single attribute
//-----------------------------------------------------------------------------
bool CXMLSerializer::SerializeAttributes( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement )
{
	// Collect the attributes to be written
	CDmAttribute **ppAttributes = ( CDmAttribute** )_alloca( pElement->AttributeCount() * sizeof( CDmAttribute* ) );
	int nAttributes = 0;
	for ( CDmAttribute *pAttribute = pElement->FirstAttribute(); pAttribute; pAttribute = pAttribute->NextAttribute() )
	{
		if ( pAttribute->IsFlagSet( FATTRIB_DONTSAVE | FATTRIB_STANDARD ) )
			continue;

		ppAttributes[ nAttributes++ ] = pAttribute;
	}

	// Now write them all out in reverse order, since FirstAttribute is actually the *last* attribute for perf reasons
	for ( int i = nAttributes - 1; i >= 0; --i )
	{
		CDmAttribute *pAttribute = ppAttributes[ i ];
		Assert( pAttribute );

		const char *pName = pAttribute->GetName();

		DmAttributeType_t nAttrType = pAttribute->GetType();
		switch( nAttrType )
		{
		default:
			{
				const char *pAttributeType = AttributeTypeName( nAttrType );
				buf.Printf( "<%s type=\"%s\">", pName, pAttributeType );
				buf.PushTab();
				if ( nAttrType >= AT_FIRST_ARRAY_TYPE )
				{
					buf.PutChar( '\n' );
					SerializeArrayAttribute( buf, pAttribute );
				}
				else
				{
					pAttribute->Serialize( buf );
				}
				buf.PopTab();
				buf.Printf( "</%s>\n", pName );
			}
			break;

		case AT_ELEMENT:
			SerializeElementAttribute( buf, dict, pName, pAttribute );
			break;

		case AT_ELEMENT_ARRAY:
			SerializeElementArrayAttribute( buf, dict, pName, pAttribute );
			break;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Serializes a single element w/ any inlines
//-----------------------------------------------------------------------------
bool CXMLSerializer::SaveElement( CUtlBuffer& buf, CDmElementSerializationDictionary& dict, CDmElement *pElement, bool bWriteDelimiters )
{
	if ( bWriteDelimiters )
	{
		char idbuf[256];
		const DmObjectId_t &id = pElement->GetId();
		UniqueIdToString( id, idbuf, sizeof(idbuf) );
		buf.Printf( "<%s name=\"%s\" id=\"%s\">\n", pElement->GetTypeString(), pElement->GetName(), idbuf );
	}
	buf.PushTab();
	SerializeAttributes( buf, dict, pElement );
	buf.PopTab();
	if ( bWriteDelimiters )
	{
		buf.Printf( "</%s>\n", pElement->GetTypeString() );
	}
	return true;
}

bool CXMLSerializer::Serialize( CUtlBuffer &outBuf, CDmElement *pRoot )
{
	SetSerializationDelimiter( &s_XMLCharConversion );

	// XML requires everything be nested in a single node
	outBuf.Printf( "<%s>\n\n", DMX_FILE_STARTING_TOKEN );

	CDmElementSerializationDictionary dict;
	dict.BuildElementList( pRoot, m_bFlatMode );

	// Save elements to buffer
	DmElementDictHandle_t i;
	for ( i = dict.FirstRootElement(); i != ELEMENT_DICT_HANDLE_INVALID; i = dict.NextRootElement(i) )
	{
		SaveElement( outBuf, dict, dict.GetRootElement( i ) );
		outBuf.Printf( "\n" );
	}

	// XML requires everything be nested in a single node
	outBuf.Printf( "</%s>\n", DMX_FILE_ENDING_TOKEN );

	SetSerializationDelimiter( NULL );
	return true;
}


//-----------------------------------------------------------------------------
// FIXME: Replace this with our own parser! Or make Parsifal not be a
// statically-linked DLL
//-----------------------------------------------------------------------------
class CXMLUnserializationState
{
public:
	CXMLUnserializationState();

	bool Unserialize( DmFileId_t fileid, CUtlBuffer &buf, DmConflictResolution_t idConflictResolution, CDmElement **ppRoot );

	int StartElement( const XMLCH *uri, const XMLCH *localName, const char *qName, LPXMLVECTOR atts );
	int Characters( const XMLCH *Chars, int cbChars);
	int EndElement( const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);

private:
	struct ParseState_t
	{
		enum ParseType_t
		{
			FILE_SCOPE = 0,
			ELEMENT,
			ATTRIBUTE,
			ATTRIBUTE_ELEMENT_REFERENCE,
			ATTRIBUTE_ELEMENT_ARRAY_REFERENCE,
			ATTRIBUTE_ARRAY_VALUE,
		};

		ParseType_t m_Type;
		DmElementDictHandle_t m_hElement;
		CDmAttribute *m_pAttribute;
	};

	// Returns the current line being parsed
	int GetCurrentLine();

	// Push/pop the various parse types
	void PushAttribute( const char *pAttributeName, CDmAttribute *pAttribute, DmElementDictHandle_t hElement, ParseState_t::ParseType_t type );
	void PopAttribute();
	void PushElement( DmElementDictHandle_t hElement );
	void PopElement();
	
	// Starts the various XML element types
	int StartFileScopeElement( const char *pElementName );
	int StartAttributeElement( const char *pAttributeName, const char *pAttributeType );
	int StartChildElement( const char *pElementType );
	
	// Ends the various XML element types
	int EndFileScopeElement( const char *pElementName );
	int EndAttributeElement( const char *pAttributeName );
	int EndChildElement( const char *pElementType );

	// Reads the various XML element types
	int CharactersElementArrayReference( const XMLCH *pChars, int cbChars );
	int CharactersElementReference( const XMLCH *pChars, int cbChars );
	int CharactersAttribute( const XMLCH *pChars, int cbChars );

	// Recomputes the topmost element due to a pop
	void RecomputeTopmostElement( );

	// Gets the topmost element
	CDmElement *GetTopmostElement();

	// Gets the top attribute (if an attribute is top of the stack)
	CDmAttribute *GetTopAttribute();

	// Creates a scene object, adds it to the element dictionary
	DmElementDictHandle_t CreateDmElement( const char *pElementType );

private:
	LPXMLPARSER m_Parser;
	CUtlStack< ParseState_t > m_ParseStack;
	DmElementDictHandle_t m_hTopmostDictHandle;
	DmElementDictHandle_t m_hRootElement;
	CDmElementDictionary m_ElementDict;
	DmConflictResolution_t m_idConflictResolution;
	DmFileId_t m_fileid;
};


//-----------------------------------------------------------------------------
// Callbacks needed by the XML parser
//-----------------------------------------------------------------------------
void ErrorHandler(LPXMLPARSER parser) 
{
	/* dummy, only for switching ErrorString etc. on */
	Assert(0);
}

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	CUtlBuffer *pBuf = (CUtlBuffer*)inputData;
	int nMaxBytes = pBuf->GetBytesRemaining();
	*cBytesActual = min( cBytes, nMaxBytes );

	// Streaming can cause BytesRemaining to be a lie
	*cBytesActual = pBuf->GetUpTo( buf, *cBytesActual );
	return (*cBytesActual < cBytes);
}

int StartElement( void *pUserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts )
{
	CXMLUnserializationState *pState = (CXMLUnserializationState*)pUserData;
	return pState->StartElement( uri, localName, (const char *)qName, atts );
}

int Characters(void *pUserData, const XMLCH *Chars, int cbChars)
{
	CXMLUnserializationState *pState = (CXMLUnserializationState*)pUserData;
	return pState->Characters( Chars, cbChars );
}

int EndElement(void *pUserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{
	CXMLUnserializationState *pState = (CXMLUnserializationState*)pUserData;
	return pState->EndElement( uri, localName, qName );
}


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CXMLUnserializationState::CXMLUnserializationState()
{
}


//-----------------------------------------------------------------------------
// Unserialization entry point
//-----------------------------------------------------------------------------
bool CXMLUnserializationState::Unserialize( DmFileId_t fileid, CUtlBuffer &buf, DmConflictResolution_t idConflictResolution, CDmElement **ppRoot )
{
	*ppRoot = NULL;
	m_hTopmostDictHandle = ELEMENT_DICT_HANDLE_INVALID;
	m_hRootElement = ELEMENT_DICT_HANDLE_INVALID;

	m_idConflictResolution = idConflictResolution;

	if (!XMLParser_Create(&m_Parser)) 
	{
		Warning( "Error creating parser!\n" );
		return false;
	}

	m_fileid = fileid;

	m_Parser->errorHandler = ErrorHandler;
	m_Parser->startElementHandler = ::StartElement;
	m_Parser->endElementHandler = ::EndElement;
	m_Parser->charactersHandler = ::Characters;
	m_Parser->UserData = this;

	bool bOk = ( XMLParser_Parse( m_Parser, cstream, &buf, 0) != XML_OK );
	if ( !bOk )
	{
		Warning("Error: %s\nLine: %d Col: %d\n", m_Parser->ErrorString, m_Parser->ErrorLine, m_Parser->ErrorColumn );
	}

	XMLParser_Free(m_Parser);

	if ( bOk )
	{
		// do this *before* getting the root, since the first element might be deleted due to id conflicts
		m_ElementDict.HookUpElementReferences();

		// Return the root element.
		*ppRoot = m_ElementDict.GetElement( m_hRootElement );

		// mark all unserialized elements as done unserializing, and call Resolve()
		for ( DmElementDictHandle_t h = m_ElementDict.FirstElement();
			h != ELEMENT_DICT_HANDLE_INVALID;
			h = m_ElementDict.NextElement( h ) )
		{
			CDmElement *pElement = m_ElementDict.GetElement( h );
			if ( !pElement )
				continue;

			CDmeElementAccessor::MarkBeingUnserialized( pElement, false );
		}
	}

	m_fileid = DMFILEID_INVALID;
	g_pDmElementFrameworkImp->RemoveCleanElementsFromDirtyList( );

	return bOk;
}


//-----------------------------------------------------------------------------
// Recomputes the topmost element due to a pop
//-----------------------------------------------------------------------------
void CXMLUnserializationState::RecomputeTopmostElement( )
{
	for ( int i = m_ParseStack.Count(); --i >= 0; )
	{
		switch( m_ParseStack[i].m_Type )
		{
		case ParseState_t::ELEMENT:
			m_hTopmostDictHandle = m_ParseStack[i].m_hElement;
			return;

		case ParseState_t::ATTRIBUTE:
			if ( m_ParseStack[i].m_pAttribute->GetType() == AT_ELEMENT )
			{
				m_hTopmostDictHandle = m_ParseStack[i].m_hElement;
				Assert( m_hTopmostDictHandle != ELEMENT_DICT_HANDLE_INVALID );
				return;
			}
			break;
		}
	}
	m_hTopmostDictHandle = ELEMENT_DICT_HANDLE_INVALID;
}


//-----------------------------------------------------------------------------
// Gets the topmost attribute (if the top is an attribute)
//-----------------------------------------------------------------------------
inline CDmAttribute *CXMLUnserializationState::GetTopAttribute()
{
	Assert( (m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE) ||
		(m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE_ARRAY_VALUE) ||
		(m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE) );

	return m_ParseStack.Top().m_pAttribute;
}


//-----------------------------------------------------------------------------
// Gets the topmost element
//-----------------------------------------------------------------------------
CDmElement *CXMLUnserializationState::GetTopmostElement()
{
	if ( m_hTopmostDictHandle == ELEMENT_DICT_HANDLE_INVALID )
		return NULL;

	return m_ElementDict.GetElement( m_hTopmostDictHandle );
}

	
//-----------------------------------------------------------------------------
// Push/pop the various parse types
//-----------------------------------------------------------------------------
void CXMLUnserializationState::PushAttribute( const char *pAttributeName, 
	CDmAttribute *pAttribute, DmElementDictHandle_t hElement, ParseState_t::ParseType_t type )
{
	Assert( type == ParseState_t::ATTRIBUTE || type == ParseState_t::ATTRIBUTE_ARRAY_VALUE || type == ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE );

	int i = m_ParseStack.Push();
	m_ParseStack[i].m_Type = type;
	m_ParseStack[i].m_pAttribute = pAttribute;
	m_ParseStack[i].m_hElement = hElement;
	if ( hElement != ELEMENT_DICT_HANDLE_INVALID )
	{
		m_hTopmostDictHandle = hElement;
	}
}

void CXMLUnserializationState::PopAttribute()
{
	Assert( m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE || 
		m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE_ARRAY_VALUE || 
		m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE );

	m_ParseStack.Pop();
	RecomputeTopmostElement();
}

void CXMLUnserializationState::PushElement( DmElementDictHandle_t hElement )
{
	int i = m_ParseStack.Push();
	m_ParseStack[i].m_Type = ParseState_t::ELEMENT;
	m_ParseStack[i].m_pAttribute = NULL;
	m_ParseStack[i].m_hElement = hElement;
	m_hTopmostDictHandle = hElement;
}

void CXMLUnserializationState::PopElement()
{
	Assert( m_ParseStack.Top().m_Type == ParseState_t::ELEMENT );
	m_ParseStack.Pop();
	RecomputeTopmostElement();
}


//-----------------------------------------------------------------------------
// Returns the current line being parsed
//-----------------------------------------------------------------------------
inline int CXMLUnserializationState::GetCurrentLine()
{
	return XMLParser_GetCurrentLine( m_Parser );
}


//-----------------------------------------------------------------------------
// Start/end the file scope XML element
//-----------------------------------------------------------------------------
int CXMLUnserializationState::StartFileScopeElement( const char *pElementName )
{
	// First thing in the file has to be the starting token
	if ( Q_stricmp( pElementName, DMX_FILE_STARTING_TOKEN ) ) 
		return XML_ABORT;

	int i = m_ParseStack.Push();
	m_ParseStack[i].m_Type = ParseState_t::FILE_SCOPE;
	return XML_OK;
}

int CXMLUnserializationState::EndFileScopeElement( const char *pElementName )
{
	// First thing in the file has to be the starting token
	if ( Q_stricmp( pElementName, DMX_FILE_ENDING_TOKEN ) ) 
		return XML_ABORT;

	m_ParseStack.Pop();
	return XML_OK;
}


//-----------------------------------------------------------------------------
// Creates a scene object, adds it to the element dictionary
//-----------------------------------------------------------------------------
DmElementDictHandle_t CXMLUnserializationState::CreateDmElement( const char *pElementType )
{
	// When reading in elements, we can also try to read in name + id
	LPXMLRUNTIMEATT nameAttr = XMLParser_GetNamedItem( m_Parser, (XMLCH*)"name" );
	LPXMLRUNTIMEATT idAttr = XMLParser_GetNamedItem( m_Parser, (XMLCH*)"id" );

	const char *pName = nameAttr ? (const char*)nameAttr->value : NULL;

	// See if we can create an element of that type
	DmElementHandle_t hElement = g_pDataModel->CreateElement( pElementType, pName, m_fileid );
	CDmElement *pElement = g_pDataModel->GetElement( hElement );
	if ( !pElement )
	{
		Warning("XML: (%d) Element uses unknown element type %s\n", GetCurrentLine(), pElementType );
		return ELEMENT_DICT_HANDLE_INVALID;
	}

	CDmeElementAccessor::MarkBeingUnserialized( pElement, true );

	DmElementDictHandle_t hDictHandle = m_ElementDict.InsertElement( pElement );

	if ( idAttr )
	{
		DmObjectId_t readId;
		UniqueIdFromString( &readId, (const char*)idAttr->value );
		m_ElementDict.SetElementId( hDictHandle, readId, m_idConflictResolution );
	}

	return hDictHandle;
}


//-----------------------------------------------------------------------------
// Start an DmAttribute XML element
//-----------------------------------------------------------------------------
int CXMLUnserializationState::StartAttributeElement( const char *pAttributeName, const char *pAttributeType )
{
	if ( m_hTopmostDictHandle == ELEMENT_DICT_HANDLE_INVALID )
	{
		Warning("XML: (%d) Tried to add attribute %s outside of an element definition scope\n", GetCurrentLine(), pAttributeType );
		return XML_ABORT;
	}

	// Look in the list of all attributes for that type.
	DmAttributeType_t nAttrType = g_pDataModel->GetAttributeTypeForName( pAttributeType );

	// If the requested type doesn't exist, then it must be an installed element type.
	if ( nAttrType == AT_UNKNOWN )
	{
		// See if we can create an element of that type
		DmElementDictHandle_t hElement = CreateDmElement( pAttributeType );
		if ( hElement == ELEMENT_DICT_HANDLE_INVALID )
			return XML_ABORT;

		CDmAttribute *pAttribute = GetTopmostElement()->AddAttribute( pAttributeName, AT_ELEMENT );
		if ( !pAttribute )
		{
			Warning("XML: (%d) Attribute %s is not requested type %s\n", GetCurrentLine(), pAttributeName, pAttributeType );
			return XML_ABORT;
		}

		CDmElement *pElement = m_ElementDict.GetElement( hElement );
		pAttribute->SetValue( pElement ? pElement->GetHandle() : DMELEMENT_HANDLE_INVALID );
		PushAttribute( pAttributeName, pAttribute, hElement, ParseState_t::ATTRIBUTE );
		return XML_OK;
	}

	CDmAttribute *pAttribute = GetTopmostElement()->AddAttribute( pAttributeName, nAttrType );
	if ( !pAttribute )
	{
		Warning("XML: (%d) Attribute %s is not requested type %s\n", GetCurrentLine(), pAttributeName, pAttributeType );
		return XML_ABORT;
	}

	// Element types are never unserialized directly, 
	// instead they are read in as object ids and are fixed up at a later time.
	ParseState_t::ParseType_t type = (nAttrType != AT_ELEMENT) ? ParseState_t::ATTRIBUTE : ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE;
	PushAttribute( pAttributeName, pAttribute, ELEMENT_DICT_HANDLE_INVALID, type );
	return XML_OK;
}

int CXMLUnserializationState::EndAttributeElement( const char *pAttributeName )
{
	// FIXME: Make this work!

//	const char *pActualAttributeName = m_ParseStack.Top().m_pElement->GetAttributeValueString( "type" );
//	if ( Q_stricmp( pAttributeName, pActualAttributeName ) )
//	{
//		Warning("XML: (%d) Attribute (%s) has mismatched ending attribute name %s\n", GetCurrentLine(), pAttributeName );
//		return XML_ABORT;
//	}

	PopAttribute( );
	return XML_OK;
}


//-----------------------------------------------------------------------------
// Start a (child) DmElement XML element
//-----------------------------------------------------------------------------
int CXMLUnserializationState::StartChildElement( const char *pElementType )
{
	// If the current top of the stack is an array attribute, then add it to the array...
	bool bIsElementArray = (m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE) && 
		( GetTopAttribute()->GetType() == AT_ELEMENT_ARRAY );

	// Look for an external child reference first
	if ( !Q_stricmp( pElementType, g_pDataModel->GetAttributeNameForType( AT_ELEMENT ) ) )
	{
		// This is using a guid to refer to a child. This can't be a root element
		if ( m_hTopmostDictHandle == ELEMENT_DICT_HANDLE_INVALID )
		{
			Warning( "XML: (%d) Child element is being defined at the root level!\n", GetCurrentLine() );
			return XML_ABORT;
		}

		if ( !bIsElementArray )
		{
			Warning( "XML: (%d) Expected array element attribute\n", GetCurrentLine() );
			return XML_ABORT;
		}

		int i = m_ParseStack.Push();
		m_ParseStack[i].m_Type = ParseState_t::ATTRIBUTE_ELEMENT_ARRAY_REFERENCE;

		// FIXME: Push state for parsing external child
		return XML_OK;
	}

	DmElementDictHandle_t hChild = CreateDmElement( pElementType );
	if ( hChild == ELEMENT_DICT_HANDLE_INVALID )
		return XML_ABORT;

	if ( bIsElementArray )
	{
		m_ElementDict.AddArrayAttribute( GetTopAttribute(), hChild );
	}
	else
	{
		// Set the root element
		if ( m_hRootElement == ELEMENT_DICT_HANDLE_INVALID )
		{
			m_hRootElement = hChild;
		}
	}

	PushElement( hChild );
	return XML_OK;
}

int CXMLUnserializationState::EndChildElement( const char *pElementType )
{
	if ( m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE_ELEMENT_ARRAY_REFERENCE )
	{
		m_ParseStack.Pop();
		return XML_OK;
	}

	Assert( m_hTopmostDictHandle == m_ParseStack.Top().m_hElement );
	const char *pActualElementType = GetTopmostElement()->GetTypeString();
	if ( Q_stricmp( pElementType, pActualElementType ) )
	{
		Warning("XML: (%d) Child element (%s) has mismatched ending element type %s\n", GetCurrentLine(), pElementType );
		return XML_ABORT;
	}

	PopElement( );
	return XML_OK;
}


//-----------------------------------------------------------------------------
// Called by the parser when we start a new XML element
//-----------------------------------------------------------------------------
int CXMLUnserializationState::StartElement( const XMLCH *uri, 
	const XMLCH *localName, const char *pElementName, LPXMLVECTOR atts )
{
	// Deal with file scope token
	if ( !m_ParseStack.Count() )
		return StartFileScopeElement( pElementName );
	
	// See if we're reading in an attribute. We are if there's an XML attribute called 'type'.
	LPXMLRUNTIMEATT att = XMLParser_GetNamedItem( m_Parser, (XMLCH*)"type" );
	if (att)
	{
		return StartAttributeElement( pElementName, (const char*)att->value );
	}

	// We're reading in an array attribute if there's no XML attribute called 'type'.

	// If the current top of the stack is an array attribute, then add it to the array...
	// To make this work, we need do
	if ( ( m_ParseStack.Top().m_Type == ParseState_t::ATTRIBUTE ) && 
		( GetTopAttribute()->GetType() != AT_ELEMENT_ARRAY ) )
	{
		PushAttribute( NULL, GetTopAttribute(), ELEMENT_DICT_HANDLE_INVALID, ParseState_t::ATTRIBUTE_ARRAY_VALUE );
		return XML_OK;
	}

	// Otherwise, we're reading elements
	return StartChildElement( pElementName );
}


//-----------------------------------------------------------------------------
// Called by the parser when we get characters inside an element array reference 
//-----------------------------------------------------------------------------
int CXMLUnserializationState::CharactersElementArrayReference( const XMLCH *pChars, int cbChars )
{
	Assert( m_ParseStack.Count() > 1 );
	ParseState_t &state = m_ParseStack[ m_ParseStack.Count() - 2 ];
	Assert( (state.m_Type == ParseState_t::ATTRIBUTE) || (state.m_Type == ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE) );

	DmObjectId_t id;
	UniqueIdFromString( &id, (const char*)pChars, cbChars );
	m_ElementDict.AddArrayAttribute( state.m_pAttribute, id );
	return XML_OK;
}


//-----------------------------------------------------------------------------
// Called by the parser when we get characters inside an element array reference 
//-----------------------------------------------------------------------------
int CXMLUnserializationState::CharactersElementReference( const XMLCH *pChars, int cbChars )
{
	DmObjectId_t id;
	UniqueIdFromString( &id, (const char*)pChars, cbChars );
	m_ElementDict.AddAttribute( GetTopAttribute(), id );
	return XML_OK;
}


//-----------------------------------------------------------------------------
// Called by the parser when we get characters inside an attribute 
//-----------------------------------------------------------------------------
int CXMLUnserializationState::CharactersAttribute( const XMLCH *pChars, int cbChars )
{
	// Not sure how to handle this one...
	CDmAttribute *pTopAttribute = GetTopAttribute();
	if ( pTopAttribute->GetType() == AT_ELEMENT )
	{
		Warning("XML (%d) : Error parsing attribute data (can't have non-attribute data in an element block)!\n", GetCurrentLine() );
		Assert( 0 );
		return XML_ABORT;
	}

	// FIXME: This totally sucks. Finish getting CUtlBuffer into the attributes.
	char *pBuf = (char*)stackalloc( cbChars + 1 );
	memcpy( pBuf, (char*)pChars, cbChars );
	pBuf[cbChars] = 0;

	CUtlBuffer buf( pBuf, cbChars, CUtlBuffer::TEXT_BUFFER | CUtlBuffer::READ_ONLY );

	bool bOk;
	if ( pTopAttribute->GetType() >= AT_FIRST_ARRAY_TYPE )
	{
		if ( m_ParseStack.Top().m_Type != ParseState_t::ATTRIBUTE_ARRAY_VALUE )
		{
			Warning("XML (%d) : Error parsing array attribute data!\n", GetCurrentLine() );
			return XML_ABORT;
		}

		bOk = pTopAttribute->UnserializeElement( buf );
	}
	else
	{
		Assert( V_stricmp( pTopAttribute->GetName(), "id" ) ); // this shouldn't be legal - I don't know why this code was here before...
		bOk = pTopAttribute->Unserialize( buf );
	}

	if ( !bOk )
	{
		Warning("XML (%d) : Error parsing attribute data!\n", GetCurrentLine() );
		return XML_ABORT;
	}

	return XML_OK;
}


//-----------------------------------------------------------------------------
// Called by the parser when we get characters inside an XML element 
//-----------------------------------------------------------------------------
int CXMLUnserializationState::Characters( const XMLCH *pChars, int cbChars )
{
	// No parse stack? This is bad
	if ( m_ParseStack.Count() == 0 )
		return XML_OK;

	// Characters only make sense if you have attributes
	switch( m_ParseStack.Top().m_Type )
	{
	case ParseState_t::ATTRIBUTE_ELEMENT_ARRAY_REFERENCE:
		return CharactersElementArrayReference( pChars, cbChars );

	case ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE:
		return CharactersElementReference( pChars, cbChars );

	case ParseState_t::ATTRIBUTE_ARRAY_VALUE:
	case ParseState_t::ATTRIBUTE:
		return CharactersAttribute( pChars, cbChars );

	default:
		// FIXME: Should I warn + abort?
		return XML_OK;
	}
}


//-----------------------------------------------------------------------------
// Called by the parser when we end an XML element
//-----------------------------------------------------------------------------
int CXMLUnserializationState::EndElement( const XMLCH *uri, const XMLCH *localName, const XMLCH *qName )
{
	switch( m_ParseStack.Top().m_Type )
	{
	case ParseState_t::FILE_SCOPE:
		return EndFileScopeElement( (const char *)qName );

	case ParseState_t::ATTRIBUTE_ELEMENT_REFERENCE:
	case ParseState_t::ATTRIBUTE:
		return EndAttributeElement( (const char *)qName );

	case ParseState_t::ATTRIBUTE_ARRAY_VALUE:
		PopAttribute();
		return XML_OK;

	case ParseState_t::ELEMENT:
	case ParseState_t::ATTRIBUTE_ELEMENT_ARRAY_REFERENCE:
		return EndChildElement( (const char *)qName );
	}

	return XML_ABORT;
}


//-----------------------------------------------------------------------------
// Main entry point for the unserialization
//-----------------------------------------------------------------------------
bool CXMLSerializer::Unserialize( CUtlBuffer &buf, const char *pEncodingName, int nEncodingVersion,
								  const char *pSourceFormatName, int nSourceFormatVersion,
								  DmFileId_t fileid, DmConflictResolution_t idConflictResolution, CDmElement **ppRoot )
{
	CXMLUnserializationState unserializer;
	bool bSuccess = unserializer.Unserialize( fileid, buf, idConflictResolution, ppRoot );
	if ( !bSuccess )
		return false;

	return g_pDataModel->UpdateUnserializedElements( pSourceFormatName, nSourceFormatVersion, fileid, idConflictResolution, ppRoot );
}
