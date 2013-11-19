/*===========================================================================
  dtdvalid.c
    References:
    http://www.w3c.org/TR/2004/REC-xml-20040204/ (XML 1.0 specification)
    http://en.wikipedia.org/wiki/Automata_theory (Automata theory)
    http://www.cogsci.ed.ac.uk/~richard/rxp.html (Very complete Validating 
    XML parser that I borrowed many ideas from)
    see parsifal.h for copyright info
===========================================================================*/

#ifdef DTDVALID_SUPPORT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "parsifal.h"
#include "dtdvalid.h"

#ifdef _MSC_VER
	#ifdef _DEBUG
		#include <crtdbg.h>
		#define _CRTDBG_MAP_ALLOC
	#endif
#define vsnprintf _vsnprintf
#endif

#ifndef STACK_PUSH
#define STACK_PUSH(stack,item) (XMLVector_Append((stack), (item)))
#define STACK_PEEK(stack) (XMLVector_Get((stack),(stack)->length-1))
#define STACK_REMOVE(stack) (XMLVector_Remove((stack), (stack)->length-1))
#define STACK_POP(stack,item) \
( ((stack)->length) ? (memcpy((item), STACK_PEEK((stack)), (stack)->itemSize), \
STACK_REMOVE((stack)), (item)) : NULL)
#endif

/* for debugging automatons: */
/*
#define PRINT_FSA
#define PRINT_FSA_ELEMENT "head"
*/

#define SMARK_FINAL 1
#define SMARK_USEFUL 2
#define SMARK_INUSE 4
#define SET_SMARK(state,m,on) ((on) ? ((state)->mark |= (m)) : ((state)->mark &= (~(m))))
#define EMPTYSTR v->parser->ErrorString
#define MAYRET(r) if (!(v->parser->XMLFlags&XMLFLAG_VALIDATION_WARNINGS)) return r;

struct FSAState {
	LPXMLVECTOR trans;
	int mark;
};

struct FSATran {
	void *label;
	struct FSAState *src;
	struct FSAState *dst;
};

struct ElementDecl {
	XMLCH *name;
	XMLCP *cp;
	int type;
	LPXMLVECTOR fsa, declAtts;
	struct FSAState *startState;
};

struct vContext {
	struct FSAState *ctx;
	struct ElementDecl *e;
};

#define CHKMEM(p) if ((p)==NULL) { return NULL; }

static const XMLCH *GetErrorString(int code);
static void Er_(LPXMLDTDVALIDATOR p, void *errCtx, XMLERRCODE code, ...);
static void Er_Hint(LPXMLDTDVALIDATOR p, void *errCtx, int n);
static struct FSAState *AddState(LPXMLDTDVALIDATOR p);
static struct FSATran *AddTran(struct FSAState *src, struct FSAState *dst, void *label);
static struct FSAState *RecurSeqCreateNFA(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next);
static struct FSAState *CreateNFA2(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next);
static struct FSAState *CreateNFA(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next);
static void UnMarkFSA(LPXMLVECTOR fsa, int mark);
static int AddEpsilonClosure(struct FSAState *base, struct FSAState *state);
static int NFAtoDFA(LPXMLDTDVALIDATOR vp, struct ElementDecl *e);
static int attcmp(const LPXMLATTDECL a1, const LPXMLATTDECL a2);
static int sattcmp(const XMLCH *key, const LPXMLATTDECL ad);
static int InitValidator(LPXMLDTDVALIDATOR vp);
static int DTDValidate_EndDTD(void *UserData);
static int AddElementDecl(LPXMLDTDVALIDATOR vp, XMLCH *name, XMLCP *cm);
static int DTDValidate_ElementDecl(void *UserData, 
								   const XMLCH *name, void *contentModel);
static struct FSAState *Validate(struct FSAState *context, XMLCH *name);
static int ValidateAttsEnum(LPXMLVECTOR e, XMLCH *val);
static int ValidateAtts(LPXMLDTDVALIDATOR vp, 
						struct ElementDecl *e, LPXMLVECTOR atts);
static void FreeDTDValidator(LPXMLDTDVALIDATOR p, int ForReuse);

static struct FSAState const forSS = {NULL, SMARK_FINAL};
static struct FSAState const * const SimpleState = &forSS;
static void const * const epsilon = &forSS;
static struct ElementDecl const AnyElement = { /* for warnings */
	NULL, NULL, XMLCTYPE_ANY, NULL, NULL, (struct FSAState*)&forSS
}; 

static const XMLCH *GetErrorString(int code)
{
	static const XMLCH * const ErrMsg[] = {
	0,
	"Out of memory",
	"Content model for '%s' doesn't allow element '%s' here",
	"Undeclared element '%s'",
	"Content model for '%s' doesn't allow character data",
	"Content model for '%s' doesn't allow it to end here",
	"Required attribute '%s' missing for element '%s'",
	"Undeclared attribute '%s' in element '%s'",
	"Value for attribute '%s' in element '%s' doesn't match its declared value",
	"Root element should be '%s'"
	};
	return (ErrMsg[code]);
}

/* TODO: fix ErrorColumn for ERR_XMLDTDV_PCDATA_NOT_ALLOWED 
   (parser has always read LT after PCDATA and thus column is inaccurate) */
static void Er_(LPXMLDTDVALIDATOR p, void *errCtx, XMLERRCODE code, ...)
{
	LPXMLPARSER parser = p->parser;
	p->ErrorCode = code;
	parser->ErrorCode = ERR_XMLP_VALIDATION; /* parser won't set its
		error strings etc. when ERR_XMLP_VALIDATION is set, however 
		it will call its errorHandler properly */
	if (parser->errorHandler) {
		int n;
		va_list aptr;
		va_start(aptr, code);		
		n = vsnprintf((char*)p->ErrorString, 128, GetErrorString(code), aptr);
		va_end(aptr);

		if (n < 0) p->ErrorString[127] = '\0';
		else if (errCtx) Er_Hint(p, errCtx, n);
		
		p->ErrorColumn =  XMLParser_GetCurrentColumn(parser);	
		p->ErrorLine = XMLParser_GetCurrentLine(parser);
		if (parser->XMLFlags&XMLFLAG_VALIDATION_WARNINGS) {
			parser->errorHandler(parser);
			parser->ErrorCode = 0;
		}
	}
}

static void Er_Hint(LPXMLDTDVALIDATOR p, void *errCtx, int n)
{
	int i=0, j;	
	XMLCH *s;
	LPXMLVECTOR v = (p->ErrorCode==ERR_XMLDTDV_ILLEGAL_ATT_VALUE) ?
		errCtx : ((struct FSAState*)errCtx)->trans;
	if (v==NULL || !v->length) return;

	do {
		if (p->ErrorCode==ERR_XMLDTDV_ILLEGAL_ATT_VALUE) {
			void **av = XMLVector_Get(v, i);
			s = *av;
		}
		else {
			struct FSATran *t = XMLVector_Get(v, i);
			s = ((XMLCP*)t->label)->name;
		}
		j = strlen(s);
		if (++i==1) {
			if (n+j+7 > 127) return;
			memcpy(p->ErrorString+n, ". Try: ", 7);
			n+=7;
		}
		else {
			if (n+j+2 > 127) break;
			p->ErrorString[n++] = ',';
			p->ErrorString[n++] = ' ';
		}
		memcpy(p->ErrorString+n, s, j);
		n+=j;
	} while (i<4 && i<v->length);
	
	if (i!=v->length && !(n+3 > 127)) {
		p->ErrorString[n++] = '.';
		p->ErrorString[n++] = '.';
		p->ErrorString[n++] = '.';
	}
	p->ErrorString[n]='\0';
}

static struct FSAState *AddState(LPXMLDTDVALIDATOR p)
{
	struct FSAState **s2, *s = XMLPool_Alloc(p->StatePool);
	CHKMEM(s);
	s->mark = 0;
	s->trans = NULL;
	s2 = XMLVector_Append(p->fsa, NULL);
	CHKMEM(s2);
	*s2 = s;
	return s;
}

static struct FSATran *AddTran(struct FSAState *src, struct FSAState *dst, void *label)
{
	struct FSATran *t;
	if (!src->trans) {
		XMLVector_Create(&src->trans, 4, sizeof(struct FSATran));
		CHKMEM(src->trans);
	}
	t = XMLVector_Append(src->trans, NULL);
	CHKMEM(t);
	t->label = label;
	t->src = src;
	t->dst = dst;
	return t;
}

static struct FSAState *RecurSeqCreateNFA(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next) 
{ /* ugly recursive solution to traverse singly linked list backwards :( BUT consider:
	 for example docbook longest seq is 8 items - using prev pointer would take about 60kB...*/
	struct FSAState *s = (cp->next) ? RecurSeqCreateNFA(vp, cp->next, next) : next;
	CHKMEM(s);
	return CreateNFA(vp, cp, s);
}

static struct FSAState *CreateNFA2(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next)
{
	struct FSAState *ns;
	CHKMEM(ns = AddState(vp));
	
	if (cp->type == XMLCTYPE_NAME) {
		CHKMEM(AddTran(ns, next, cp));
	}
	else {
		struct FSAState *s;
		if (cp->type == XMLCTYPE_SEQ) {
			CHKMEM(s = RecurSeqCreateNFA(vp, cp->children, next));
			CHKMEM(AddTran(ns, s, (void*)epsilon));
		}
		else {
			for(cp = cp->children; cp; cp = cp->next) {
				CHKMEM(s = CreateNFA(vp, cp, next));
				CHKMEM(AddTran(ns, s, (void*)epsilon));
			}
		}
	}
	return ns;
}

static struct FSAState *CreateNFA(LPXMLDTDVALIDATOR vp, XMLCP *cp, struct FSAState *next)
{
	struct FSAState *sp, *n1, *n2;
	switch(cp->rep) {
		case 0: 
			return CreateNFA2(vp, cp, next);
		case '*':
			CHKMEM(n1 = AddState(vp));
			CHKMEM(sp = CreateNFA2(vp, cp, n1));
			CHKMEM(AddTran(n1, sp, (void*)epsilon));
			CHKMEM(AddTran(n1, next, (void*)epsilon));
			return n1;
		case '+':
			CHKMEM(n1 = AddState(vp));
			CHKMEM(n2 = AddState(vp));
			CHKMEM(sp = CreateNFA2(vp, cp, n2));
			CHKMEM(AddTran(n1, sp, (void*)epsilon));
			CHKMEM(AddTran(n2, sp, (void*)epsilon));
			CHKMEM(AddTran(n2, next, (void*)epsilon));
			return n1;
		case '?':
			CHKMEM(n1 = AddState(vp));
			CHKMEM(sp = CreateNFA2(vp, cp, next));
			CHKMEM(AddTran(n1, sp, (void*)epsilon));
			CHKMEM(AddTran(n1, next, (void*)epsilon));
			return n1;
	}
	return NULL;
}

#ifdef PRINT_FSA
static void PrintFSA(struct ElementDecl *e, XMLCH *type)
{
	int i, j;
	struct FSAState *s, **ss;
	struct FSATran *t;

#ifdef PRINT_FSA_ELEMENT /* outputs automaton only for this element*/
	if (strcmp(e->name, PRINT_FSA_ELEMENT)) return;
#endif

	printf("%s for element \"%s\":\n", type, e->name);

	for (i=0; i<e->fsa->length; i++) {
		ss = XMLVector_Get(e->fsa, i);
		s = *ss;
		
		if(s == e->startState || s->mark & SMARK_FINAL) {
			if (s==e->startState) printf("%p [Start]\n", s);
			if (s->mark & SMARK_FINAL) printf("%p [End]\n", s);
		}
		else printf("%p\n", s);
		
		if (s->trans) {
			for (j=0; j<s->trans->length; j++) {
				t = XMLVector_Get(s->trans, j);
				if (t->label == epsilon)
					printf("  [epsilon] -> %p\n", t->dst);
				else
					printf("  %s -> %p\n", ((XMLCP*)t->label)->name, t->dst);
			}
		}
	}
}
#endif

static void UnMarkFSA(LPXMLVECTOR fsa, int mark)
{
	struct FSAState **s, **se;
	se = (struct FSAState **)_XMLVector_GetIterP(fsa, s);
	for (;s!=se;s++) (*s)->mark &= ~mark;
}

static int AddEpsilonClosure(struct FSAState *base, struct FSAState *state) 
{
	int i;
	struct FSATran *t1, *t2, *te2;

	if (state->mark & SMARK_INUSE) return 1;
	SET_SMARK(state, SMARK_INUSE, 1);
	
	if (state->mark & SMARK_FINAL) {		
		SET_SMARK(base, SMARK_FINAL, 1);
	}
	if (!state->trans) return 1;

	for (i=0; i<state->trans->length; i++) {
		t1 = XMLVector_Get(state->trans, i);
		if (t1->label == epsilon) {
			if (!AddEpsilonClosure(base, t1->dst)) return 0;
		}
		else {
			te2 = (struct FSATran *)_XMLVector_GetIterP(base->trans, t2);
			for (; t2!=te2; t2++) {
				if(t2->label == t1->label &&
					t2->dst == t1->dst) break;
			}
			if(t2 == te2) {
				if (!AddTran(base, t1->dst, t1->label)) return 0;
			}
		}
	}
	return 1;
}

static int NFAtoDFA(LPXMLDTDVALIDATOR vp, struct ElementDecl *e)
{
	struct FSAState **sw, **s, **se;
	struct FSATran *t, *te;
	LPXMLVECTOR o;
	int i;

	XMLVector_Create(&o, 0, sizeof(struct FSAState*));
	if (!o) return 0;

	SET_SMARK(e->startState, SMARK_USEFUL, 1);

	se = (struct FSAState **)_XMLVector_GetIterP(e->fsa, s);
	for (;s!=se;s++) {
		if ((*s)->trans) {
			te = (struct FSATran *)_XMLVector_GetIterP((*s)->trans, t);
			for (; t!=te; t++) {
				if (t->label != epsilon) {
					SET_SMARK(t->dst, SMARK_USEFUL, 1);
				}
			}
		}
	}

	se = (struct FSAState **)_XMLVector_GetIterP(e->fsa, s);
	for (;s!=se;s++) {
		if ((*s)->mark & SMARK_USEFUL) {		
			SET_SMARK((*s), SMARK_INUSE, 1);
			if ((*s)->trans) {		
				for (i=0; i<(*s)->trans->length; i++) {		
					t = XMLVector_Get((*s)->trans, i);
					if (t->label == epsilon) {
						if (!AddEpsilonClosure(*s, t->dst)) return 0;
					}
				}
			}
			UnMarkFSA(e->fsa, SMARK_INUSE);
		}
	}

	se = (struct FSAState **)_XMLVector_GetIterP(e->fsa, s);
	for (;s!=se;s++) {
		if ((*s)->mark & SMARK_USEFUL) {
			if ((*s)->trans) {
				for (i=0; i<(*s)->trans->length; i++) {
					t = XMLVector_Get((*s)->trans, i);
					if (t->label == epsilon) {	
						if (!XMLVector_Remove((*s)->trans, i)) return 0;
						i--;
					}
				}
			}
			sw = XMLVector_Append(o, NULL);
			if (!sw) return 0;
			*sw = *s;
		}
		else if ((*s)->trans) {
			XMLVector_Free((*s)->trans);
		}
	}
	XMLVector_Free(e->fsa);
	e->fsa = o;
	return 1;
}

/* sorts XMLATTDECL vector by special rules:
   XMLATTDECL_DEF_REQUIRED atts will be first and
   others will follow them sorted by name (for
   bsearch - see ValidateAtts()) */
static int attcmp(const LPXMLATTDECL a1, const LPXMLATTDECL a2)
{
	if (a1->defaultDecl == XMLATTDECL_DEF_REQUIRED) {
		return (a2->defaultDecl == XMLATTDECL_DEF_REQUIRED) ? 0 : -1;
	}
	if (a2->defaultDecl == XMLATTDECL_DEF_REQUIRED) {
		return (a1->defaultDecl == XMLATTDECL_DEF_REQUIRED) ? 0 : 1;
	}
	return strcmp(a1->name, a2->name);
}

static int sattcmp(const XMLCH *key, const LPXMLATTDECL ad)
{
	return strcmp(key, ad->name);
}

static int InitValidator(LPXMLDTDVALIDATOR v) 
{
	struct ElementDecl *e, *ee;
	struct FSAState *s;
	LPXMLVECTOR *declAtts;

	ee = (struct ElementDecl *)_XMLVector_GetIterP(v->ElementDecls, e);
	for (; e!=ee; e++) {
		/* important: every 1. content particle contains a pointer to
		hastablebucket in its name, we MUST assign hashtable data here,
		we cannot alter the cpNames table when DTD parsing is in progress! */
		
		if (((XMLHTABLEBUCKET*)e->cp->name)->data != EMPTYSTR)
			continue; /* duplicate declaration */
		((XMLHTABLEBUCKET*)e->cp->name)->data = e;
	
		switch (e->type) {
			case XMLCTYPE_ANY: 
			case XMLCTYPE_EMPTY: break;
			case XMLCTYPE_MIXED: if (!e->cp->children) /* simple #PCDATA */
									 break; 
				/* note that we can't easily simplify the (e) type
				content models (which is actually XMLCTYPE_CHOICE) 
				- since we must allow (e)* or ((e))? etc. */
			default:
			v->fsa = XMLVector_Create(&e->fsa, 4, sizeof(struct FSAState*));
			if (!e->fsa) return 0;
			s = AddState(v);
			if (!s) return 0;
			SET_SMARK(s, SMARK_FINAL, 1);
			e->startState = CreateNFA(v, e->cp, s);
			if (!e->startState) return 0;

#ifdef PRINT_FSA
			PrintFSA(e, "NFA");
			if (!NFAtoDFA(v, e)) return 0;
			PrintFSA(e, "DFA");		
#else	
			if (!NFAtoDFA(v, e)) return 0;
#endif
		}
		if (v->parser->prt->declAttTable) {
			declAtts = XMLHTable_Lookup(v->parser->prt->declAttTable, e->name);
			if (declAtts) {
				e->declAtts = *(declAtts+1);
				if (e->declAtts->length > 1) 
					qsort((void*)e->declAtts->array, e->declAtts->length, sizeof(XMLATTDECL), 
						(int(*)(const void*, const void*))attcmp);
			}
		}
	}

	v->ElementTable = v->parser->prt->cpNames;
	v->cpNodesPool = v->parser->prt->cpNodesPool; 
	return 1;
}

static int DTDValidate_EndDTD(void *UserData)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!InitValidator(v)) {
		if (!v->ErrorCode) Er_(v, NULL, ERR_XMLDTDV_MEMORY_ALLOC);
		return XML_ABORT;
	} 
	return (v->endDTDHandler) ? v->endDTDHandler(v) : 0;
}

static int AddElementDecl(LPXMLDTDVALIDATOR vp, XMLCH *name, XMLCP *cm)
{
	struct ElementDecl *e;
	e = XMLVector_Append(vp->ElementDecls, NULL);
	if (!e) { 
		Er_(vp, NULL, ERR_XMLDTDV_MEMORY_ALLOC);
		return 0;
	}
	/* We set the initial startState value to SimpleState and not to NULL;
	this is good for types ANY, EMPTY or simple (#PCDATA) */
	e->startState = (struct FSAState*)SimpleState; 
	e->fsa = e->declAtts = NULL;
	e->name = name;
	e->type = cm->type;
	e->cp = cm;
	return 1;
}

static int DTDValidate_ElementDecl(void *UserData, 
								   const XMLCH *name, void *contentModel)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!AddElementDecl(v, (XMLCH*)name, contentModel))
		return XML_ABORT;
	return (v->elementDeclHandler) ? v->elementDeclHandler(v, name, contentModel) : 0;
}

static struct FSAState *Validate(struct FSAState *context, XMLCH *name) 
{
	if (context->trans) {
		struct FSATran *t, *te;
		te = (struct FSATran *)_XMLVector_GetIterP(context->trans, t);
		for (; t!=te; t++) {	
			if (!strcmp(((XMLCP*)t->label)->name, name)) return t->dst;	
		}
	}
	return NULL;
}

static int ValidateAttsEnum(LPXMLVECTOR e, XMLCH *val)
{
	int i;
	void **s;
	for (i=0; i<e->length; i++) {
		s = XMLVector_Get(e, i);
		if (!strcmp((XMLCH*)*s, val)) return 1;
	}
	return 0;
}

static int ValidateAtts(LPXMLDTDVALIDATOR v, 
						struct ElementDecl *e, LPXMLVECTOR atts)
{
	LPXMLATTDECL da, da2;
	LPXMLRUNTIMEATT a, ae;
	int numAtts;

	if (e->declAtts) {		
		da2 = (LPXMLATTDECL)_XMLVector_GetIterP(e->declAtts, da);
		for (;da!=da2;da++) {
			if (da->defaultDecl != XMLATTDECL_DEF_REQUIRED) break;
			a = XMLParser_GetNamedItem(v->parser, da->name);
			if (!a) {
				Er_(v, NULL, ERR_XMLDTDV_REQUIRED_ATT_MISSING, da->name, e->name);
				MAYRET(0);
				continue;
			}
			if (da->type > XMLATTDECL_TYPE_NMTOKENS && 
				!ValidateAttsEnum(da->pExt, a->value)) {
					Er_(v, da->pExt, ERR_XMLDTDV_ILLEGAL_ATT_VALUE, a->qname, e->name);
					MAYRET(0);
			}
			a->value = EMPTYSTR; /* hack for marking this attribute processed */		
		}
		numAtts = da2-da; /* maybe 0 (only #REQUIRED atts declared) */
	}
	else numAtts = 0; /* no declared atts */

	ae = (LPXMLRUNTIMEATT)_XMLVector_GetIterP(atts, a);
	for (;a!=ae;a++) {
		if (_XMLParser_AttIsDefaulted(a))
			break; /* defaulted atts are always at the end so don't test further */
		else if (a->value == EMPTYSTR) /* already processed */
			 a->value = a->valBuf.str;
		else {
			da2 = (numAtts) ? bsearch(a->qname, da, numAtts, 
				sizeof(XMLATTDECL), (int(*)(const void*, const void*))sattcmp) : NULL;
			if (!da2) {
				Er_(v, NULL, ERR_XMLDTDV_UNDECLARED_ATT, a->qname, e->name);
				MAYRET(0);
			}
			else if (da2->defaultDecl == XMLATTDECL_DEF_FIXED &&
					strcmp(a->value, da2->value)) {
						Er_(v, NULL, ERR_XMLDTDV_ILLEGAL_ATT_VALUE, a->qname, e->name);
						MAYRET(0);
			}
			else if (da2->type > XMLATTDECL_TYPE_NMTOKENS && 
				!ValidateAttsEnum(da2->pExt, a->value)) {
					Er_(v, da2->pExt, ERR_XMLDTDV_ILLEGAL_ATT_VALUE, a->qname, e->name);
					MAYRET(0);
			}
		}
	}
	return 1;
}

int XMLAPI DTDValidate_Characters(void *UserData, 
								  const XMLCH *chars, int cbSize)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	struct vContext *c = STACK_PEEK(v->ContextStack);
	if (c && !(c->e->type ==  XMLCTYPE_MIXED || c->e->type == XMLCTYPE_ANY)) {
		Er_(v, NULL, ERR_XMLDTDV_PCDATA_NOT_ALLOWED, c->e->name);
		MAYRET(XML_ABORT);
	}
	return (v->charactersHandler) ? v->charactersHandler(v, chars, cbSize) : 0;
}

int XMLAPI DTDValidate_IgnorableWhitespace(void *UserData, 
										   const XMLCH *chars, int cbSize)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	struct vContext *c = STACK_PEEK(v->ContextStack);
	if (c && (c->e->type ==  XMLCTYPE_MIXED || c->e->type == XMLCTYPE_ANY)) {
		return (v->charactersHandler) ? v->charactersHandler(v, chars, cbSize) : 0;
	}
	return (v->ignorableWhitespaceHandler) ? 
		v->ignorableWhitespaceHandler(v, chars, cbSize) : 0;
}

int XMLAPI DTDValidate_EndElement(void *UserData, const XMLCH *uri, 
								  const XMLCH *localName, const XMLCH *qName)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	struct vContext *c = STACK_PEEK(v->ContextStack);
	if (c) {
		if (!(c->ctx->mark & SMARK_FINAL)) {
			Er_(v, c->ctx, ERR_XMLDTDV_CONTENT_MODEL_CANNOT_END, c->e->name);
			MAYRET(XML_ABORT);
		}
		STACK_REMOVE(v->ContextStack);
	}
	return (v->endElementHandler) ? v->endElementHandler(v, uri, localName, qName) : 0;
}

int XMLAPI DTDValidate_StartElement(void *UserData, const XMLCH *uri, 
									const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	struct vContext *c;
	void *d;
	
	if (v->ContextStack->length) {
		c = STACK_PEEK(v->ContextStack);
		if (c->e->type != XMLCTYPE_ANY) {
			struct FSAState *ctx = Validate(c->ctx, (XMLCH*)qName);
			if (!ctx) {
				Er_(v, c->ctx, ERR_XMLDTDV_ELEMENT_NOT_ALLOWED, c->e->name, qName);
				MAYRET(XML_ABORT);
			}
			else c->ctx = ctx;
		}
	}
	else if (v->startElementHandlerFilter == DTDValidate_StartElement &&
		v->parser->prt->doctypeStr) {
		/* test for ERR_XMLDTDV_ROOTELEMENT_MISMATCH */
		if (strcmp(qName, v->parser->prt->doctypeStr)) {
			Er_(v, NULL, ERR_XMLDTDV_ROOTELEMENT_MISMATCH, v->parser->prt->doctypeStr);
			MAYRET(XML_ABORT);
		}
	}
	
	if (!(c = STACK_PUSH(v->ContextStack, NULL))) {
		Er_(v, NULL, ERR_XMLDTDV_MEMORY_ALLOC);
		return XML_ABORT;
	}

	d = (v->ElementTable) ? 
		XMLHTable_Lookup(v->ElementTable, (char*)qName) : NULL; /* catches the case
															of missing DTD too */
	if (!d || d == EMPTYSTR) { /* note that we're also testing the empty string 
		(element is in hashtable because it was used in cp but it wasn't declared) */
		Er_(v, NULL, ERR_XMLDTDV_UNDECLARED_ELEMENT, qName);
		MAYRET(XML_ABORT);
		c->e = (struct ElementDecl*)&AnyElement;
		c->ctx = c->e->startState;
		return (v->startElementHandler) ?
			v->startElementHandler(v, uri, localName, qName, atts) : 0;
	}
	c->e = d;
	c->ctx = c->e->startState;
	if (!ValidateAtts(v, c->e, atts)) return XML_ABORT;
	return (v->startElementHandler) ?
		v->startElementHandler(v, uri, localName, qName, atts) : 0;
}

static void FreeDTDValidator(LPXMLDTDVALIDATOR p, int ForReuse)
{
	if (p->ElementDecls) {
		struct ElementDecl *pEl, *pEnd;
		struct FSAState **pS, **pSEnd;
		pEnd = (struct ElementDecl *)_XMLVector_GetIterP(p->ElementDecls, pEl);
		for (; pEl!=pEnd; pEl++) {
			if (pEl->fsa) {
				pSEnd = (struct FSAState **)_XMLVector_GetIterP(pEl->fsa, pS);
				for (; pS!=pSEnd; pS++) {
					if ((*pS)->trans) XMLVector_Free((*pS)->trans);
				}
				XMLVector_Free(pEl->fsa);
			}
		}
		if (ForReuse) _XMLVector_RemoveAll(p->ElementDecls);
		else XMLVector_Free(p->ElementDecls);
	}
	
	/* these are allocated by the parser when XMLFLAG_REPORT_DTD_EXT
		is set, we're responsible for freeing them: */
	if (p->cpNodesPool) 
		XMLPool_FreePool(p->cpNodesPool);
	if (p->ElementTable) 
		XMLHTable_Destroy(p->ElementTable, NULL, 1);
	
	if (ForReuse) {
		if (p->StatePool && p->StatePool->blocksAllocated) {
			XMLPool_FreePool(p->StatePool);
			p->StatePool = XMLPool_Create(sizeof(struct FSAState), 16);
			if (!p->StatePool) Er_(p, NULL, ERR_XMLDTDV_MEMORY_ALLOC);
		}
		return;
	}
	
	if (p->StatePool) 
		XMLPool_FreePool(p->StatePool);
	if (p->ContextStack) 
		XMLVector_Free(p->ContextStack);
	free(p);
}

int XMLAPI XMLParser_ParseValidateDTD(LPXMLDTDVALIDATOR dtd, LPXMLPARSER parser,
			LPFNINPUTSRC inputSrc, void *inputData, const XMLCH *encoding)
{
	int ret;
	if (!dtd || !parser) return 0;
	dtd->parser = parser;
	dtd->ErrorCode = dtd->ErrorColumn = dtd->ErrorLine = 0; /* must be
		set before FreeDTDValidator */
	if (*dtd->ErrorString) dtd->ErrorString[0] = '\0';

	if (dtd->ContextStack->length) 
		_XMLVector_RemoveAll(dtd->ContextStack);
	
	if (dtd->cpNodesPool) {
		FreeDTDValidator(dtd, 1);
		if (dtd->ErrorCode) return 0;
	}	
	dtd->ElementTable = NULL;
	dtd->cpNodesPool = NULL;
	
	/* save handlers: */
	dtd->startElementHandler = parser->startElementHandler;
	dtd->endElementHandler = parser->endElementHandler;
	dtd->charactersHandler = parser->charactersHandler;
	dtd->ignorableWhitespaceHandler = parser->ignorableWhitespaceHandler;	
	dtd->elementDeclHandler = parser->elementDeclHandler;
	dtd->endDTDHandler = parser->endDTDHandler;
	/* set the validating ones: */
	parser->startElementHandler = dtd->startElementHandlerFilter;
	parser->endElementHandler = dtd->endElementHandlerFilter;
	parser->charactersHandler = dtd->charactersHandlerFilter;
	parser->ignorableWhitespaceHandler = dtd->ignorableWhitespaceHandlerFilter;
	parser->elementDeclHandler = DTDValidate_ElementDecl;
	parser->endDTDHandler = DTDValidate_EndDTD;	
	
	_XMLParser_SetFlag(parser, XMLFLAG_REPORT_DTD_EXT, 1);
	parser->UserData = dtd;

	ret = XMLParser_Parse(parser, inputSrc, inputData, encoding);
	if (!ret) {
		/* Assign ElementTable and content particle pool 'cos validator 
		will be responsible for freeing them and endDTD might not be called */
		if (dtd->ElementTable!=parser->prt->cpNames)
			dtd->ElementTable = parser->prt->cpNames;
		if (dtd->cpNodesPool!=parser->prt->cpNodesPool)
			dtd->cpNodesPool = parser->prt->cpNodesPool;
	}
	
	if (parser->prt->doctypeStr) {
		free(parser->prt->doctypeStr);
		parser->prt->doctypeStr = NULL;
	}

	_XMLParser_SetFlag(parser, XMLFLAG_REPORT_DTD_EXT, 0);
	
	/* restore handlers: */
	parser->startElementHandler = dtd->startElementHandler;
	parser->endElementHandler = dtd->endElementHandler;
	parser->charactersHandler = dtd->charactersHandler;
	parser->ignorableWhitespaceHandler = dtd->ignorableWhitespaceHandler;
	parser->elementDeclHandler = dtd->elementDeclHandler;
	parser->endDTDHandler = dtd->endDTDHandler;	
	return ret;
}

void XMLAPI XMLParser_FreeDTDValidator(LPXMLDTDVALIDATOR dtd)
{
	if (dtd) FreeDTDValidator(dtd, 0);
}

LPXMLDTDVALIDATOR XMLAPI XMLParser_CreateDTDValidator(void)
{
	LPXMLDTDVALIDATOR v = malloc(sizeof(XMLDTDVALIDATOR));
	if (!v) return NULL;
	memset(v, 0, sizeof(XMLDTDVALIDATOR));
	XMLVector_Create(&v->ElementDecls, 16, sizeof(struct ElementDecl));
	if (!v->ElementDecls) goto ExitErr;
	XMLVector_Create(&v->ContextStack, 0, sizeof(struct vContext));
	if (!v->ContextStack) goto ExitErr;
	v->StatePool = XMLPool_Create(sizeof(struct FSAState), 16);
	if (!v->StatePool) goto ExitErr;
	v->startElementHandlerFilter = DTDValidate_StartElement;
	v->endElementHandlerFilter = DTDValidate_EndElement;
	v->charactersHandlerFilter = DTDValidate_Characters;
	v->ignorableWhitespaceHandlerFilter = DTDValidate_IgnorableWhitespace; 
	return v;

ExitErr:
	if (v->ElementDecls) XMLVector_Free(v->ElementDecls);
	if (v->ContextStack) XMLVector_Free(v->ContextStack);
	if (v->StatePool) XMLPool_FreePool(v->StatePool);
	free(v);
	return NULL;
}

#endif /* DTDVALID_SUPPORT */


