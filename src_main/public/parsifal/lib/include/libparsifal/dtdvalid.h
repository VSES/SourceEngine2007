/*===========================================================================
  dtdvalid.h
    see parsifal.h for copyright info
===========================================================================*/
#ifndef DTDVALID__H
#define DTDVALID__H

typedef struct tagXMLDTDVALIDATOR {
    LPXMLPARSER parser;
    LPXMLVECTOR ElementDecls, fsa, ContextStack;
    LPXMLPOOL StatePool, cpNodesPool;
    LPXMLHTABLE ElementTable;
    XML_EVENT_HANDLER endDTDHandler;
    XML_CHARACTERS_HANDLER charactersHandler;
    XML_CHARACTERS_HANDLER ignorableWhitespaceHandler;
    XML_START_ELEMENT_HANDLER startElementHandler;
    XML_END_ELEMENT_HANDLER endElementHandler;
    XML_ELEMENTDECL_HANDLER elementDeclHandler;
    XML_CHARACTERS_HANDLER charactersHandlerFilter;
    XML_CHARACTERS_HANDLER ignorableWhitespaceHandlerFilter;
    XML_START_ELEMENT_HANDLER startElementHandlerFilter;
    XML_END_ELEMENT_HANDLER endElementHandlerFilter;
    void *UserData;
    int UserFlag;
    int ErrorCode;
    int ErrorLine;
    int ErrorColumn;
    XMLCH ErrorString[128];
} XMLDTDVALIDATOR, *LPXMLDTDVALIDATOR;

#define ERR_XMLDTDV_MEMORY_ALLOC 1
#define ERR_XMLDTDV_ELEMENT_NOT_ALLOWED 2
#define ERR_XMLDTDV_UNDECLARED_ELEMENT 3
#define ERR_XMLDTDV_PCDATA_NOT_ALLOWED 4
#define ERR_XMLDTDV_CONTENT_MODEL_CANNOT_END 5
#define ERR_XMLDTDV_REQUIRED_ATT_MISSING 6
#define ERR_XMLDTDV_UNDECLARED_ATT 7
#define ERR_XMLDTDV_ILLEGAL_ATT_VALUE 8
#define ERR_XMLDTDV_ROOTELEMENT_MISMATCH 9

#ifdef __cplusplus
   extern "C" {
#endif

LPXMLDTDVALIDATOR XMLAPI XMLParser_CreateDTDValidator(void);
void XMLAPI XMLParser_FreeDTDValidator(LPXMLDTDVALIDATOR dtd);
int XMLAPI XMLParser_ParseValidateDTD(LPXMLDTDVALIDATOR dtd,
        LPXMLPARSER parser, LPFNINPUTSRC inputSrc, void *inputData, const XMLCH *encoding);
int XMLAPI DTDValidate_Characters(void *UserData, 
        const XMLCH *chars, int cbSize);
int XMLAPI DTDValidate_IgnorableWhitespace(void *UserData, 
        const XMLCH *chars, int cbSize);
int XMLAPI DTDValidate_EndElement(void *UserData, const XMLCH *uri, 
        const XMLCH *localName, const XMLCH *qName);
int XMLAPI DTDValidate_StartElement(void *UserData, const XMLCH *uri, 
        const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);

#ifdef __cplusplus
   }
#endif /* __cplusplus */
#endif /* DTDVALID__H */
