/*===========================================================================
  xmldtd.h
    contain definitions that are DTD specific
    (separated from parsifal.h for clarity)
    see parsifal.h for copyright info
===========================================================================*/

#ifndef XMLDTD__H
#define XMLDTD__H

/* for XML_ATTRIBUTEDECL_HANDLER valueDef param: */
#define XMLATTDECL_DEF_FIXED 1
#define XMLATTDECL_DEF_REQUIRED 2
#define XMLATTDECL_DEF_IMPLIED 3

/* for XML_ATTRIBUTEDECL_HANDLER type param: */
#define XMLATTDECL_TYPE_CDATA 1
#define XMLATTDECL_TYPE_ID 2
#define XMLATTDECL_TYPE_IDREF 3
#define XMLATTDECL_TYPE_IDREFS 4
#define XMLATTDECL_TYPE_ENTITY 5
#define XMLATTDECL_TYPE_ENTITIES 6
#define XMLATTDECL_TYPE_NMTOKEN 7
#define XMLATTDECL_TYPE_NMTOKENS 8
#define XMLATTDECL_TYPE_NOTATION 9
#define XMLATTDECL_TYPE_ENUMERATED 10

/* XMLATTDECL represents DTD declared attribute. Array of XMLATTDECLs
is stored in declAttTable hashtable for expansion of default attribute 
values (value member !NULL) or special normalization rules; if type
isn't CDATA normalization follow rules specified in XMLSPEC.
declAttTable will contain all declared attributes if XMLFLAG_REPORT_DTD_EXT
is true. see also XMLspec 3.3.3 Attribute-Value Normalization */
typedef struct tagXMLATTDECL
{
	XMLCH *name, *value;
	int nameLen, prefixLen, valueLen, type, defaultDecl;
	void *pExt;
} XMLATTDECL, *LPXMLATTDECL;

/* cp content model types and tree node XMLCP type
(used when XMLFLAG_REPORT_DTD_EXT is true) */
#define XMLCTYPE_CHOICE 1
#define XMLCTYPE_SEQ 2
#define XMLCTYPE_MIXED 3
#define XMLCTYPE_EMPTY 4
#define XMLCTYPE_ANY 5
#define XMLCTYPE_NAME 6

typedef struct tagXMLCP
{	
	void *name;
	int type;
	XMLCH rep;
	struct tagXMLCP *children, *next;
} XMLCP, *LPXMLCP;

#endif /* XMLDTD__H */



