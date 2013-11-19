/*
 * Copyright 1995, 2003 Perforce Software.  All rights reserved.
 */

/*
 * Spec.h - spec manipulation
 *
 * Description:
 *
 *	A 'spec' is one of those dumb ascii forms that user edits
 *	to specify various information -- clients, labels, etc.
 *
 *	The Spec class is a spec definition, indicating what fields
 *	there are and their format.  For formatting and parsing, 
 *	Spec::Format() and Spec::Parse() expect a subclassed
 *	SpecData object, with SpecData::Get() and SpecData::Put()
 *	defined for moving data between the actual object and the spec.
 *
 *	The Spec object contains SpecElems to describe each element
 *	of a spec.
 *
 *	A Spec can be encoded into a simple string for passing between
 *	processes.  Spec::Encode() and Spec::Decode() do this.  They
 *	are built for interoperability.  Namely, Decode() ignores fields
 *	it doesn't understand.
 *
 *	Finally, a Spec is also be represented as a 'jobspec': this is
 *	actually a form (formatted by Spec) that describes another Spec
 *	(in jobspec's case, a job's Spec).  The SpecData that does this 
 *	is part of the jobspec code, but that uses Spec::Add(), Get() 
 *	and SpecElem::Fmt*() and Set*() for low level construction/
 *	examination of the spec.
 *
 * Spec definition strings:
 *
 *	A Spec is described by a definition string, parsed by
 *	Spec::Encode() and regenerated (if needed) by Spec::Decode().
 *	Each item on the form is describe by a substring separated by
 *	other items by ";;".  That substring contains a number of codes
 *	separated by ";".  Each code is one of the following:
 *
 *	type:X		field type
 *
 *	  type:word	single line with N words each
 *	  type:wlist	list of lines with N words each
 *	  type:select	single line with a word selected from a list
 *	  type:line	single line of arbitrary data
 *	  type:llist	list of lines of arbitrary data
 *	  type:date	single line with a date on it
 *	  type:text	block of text spanning any number of lines
 *	  type:bulk	text that doesn't get indexed (for jobs)
 *
 *	opt:X		how optional the field is
 *
 *	  opt:optional	not required
 *	  opt:default	required, and has a default
 *	  opt:required	required, but no default
 *	  opt:once	read-only; set automatically before user gets it
 *	  opt:always	read-only; field set automatically after user
 *
 *	code:X		a unique numeric code identifying the field
 *	len:X		advisory length for displaying field
 *	pre=X		preset value for opt:once, opt:always
 *	ro		old name for opt:always
 *	rq		old name for opt:required
 *	seq:X		advisory sequence for display field
 *	val:X		/-separated list of values for type:select
 *	words:X		the number of words for a word/wlist field
 *
 *	fmt:X		advisory format for displaying field
 *
 *	  fmt:none	normal (full width)
 *	  fmt:L		left half only
 *	  fmt:R		right half only; if follows L goes on same line
 *	  fmt:I		indented
 *
 * Class Defined:
 *
 *	Spec - the definition of a spec, for parsing and formatting
 *	SpecElem - the definition of a single item type in a spec
 *	SpecData - a spec-specific formatter/parser helper
 * 	SpecWords -- array of words in a spec value, allowing surrounding "'s
 *	SpecDataTable -- a SpecData interface to a StrDict 
 *
 * Virtual methods, to be defined by caller:
 *
 *	SpecData::Get() - get a data value for stuffing into a spec
 *	SpecData::Set() - set a data value parsed from a spec
 *
 * Public methods:
 *
 *	Spec::Add() -- add a single SpecElem manually, with default values
 * 	Spec::Decode() -- decode a spec definition from a string
 * 	Spec::Encode() -- encode a spec definition in a transmittable string
 *	Spec::Find() -- find a SpecElem in the spec
 *	Spec::Get() -- find n'th SpecElem in the spec
 *	Spec::GetComment() -- return the spec's comment string
 *	Spec::Format() -- turn SpecData into a spec string
 *	Spec::Parse() -- parse a spec string into SpecData
 *	Spec::ParseNoValid() -- parse without validating 'select' items
 *	Spec::SetComment() -- set the spec's comment string
 *
 *	SpecElem::FmtOpt() - format the SpecOpt for jobspec
 *	SpecElem::FmtType() - format the SpecType for jobspec
 *	SpecElem::FmtFmt() - format the SpecFmt for jobspec
 *	SpecElem::Is*() - ask various questions about the SpecType
 *	SpecElem::SetOpt() - parse the SpecOpt from a jobspec
 *	SpecElem::SetType() - format the SpecOpt for a jobspec
 *	SpecElem::Compare() - compare SpecElems from different specs
 */

#ifndef _spec_h_
#define _spec_h_

class VarArray;
class SpecData;
class SpecElem;
class StrBufDict;

const int SpecWordsMax = 10; // for SDT_WORD, WLIST, SELECT

enum SpecType {
	SDT_WORD,	// single line, N words
	SDT_WLIST,	// multiple lines, N words
	SDT_SELECT,	// SDT_WORD from a list of words
	SDT_LINE,	// single line of text (arbitrary words)
	SDT_LLIST,	// multiple lines of text (arbitrary words)
	SDT_DATE,	// SDT_LINE that is a date
	SDT_TEXT,	// block of text,
	SDT_BULK	// SDT_TEXT not indexed
} ;

enum SpecOpt {
	SDO_OPTIONAL,	// not required, user updatable, no default
	SDO_DEFAULT,	// not required, user updatable, default provided
	SDO_REQUIRED,	// required, user updatable, default provided
	SDO_ONCE,	// required, not updatable, set once after creation 
	SDO_ALWAYS,	// required, not updatable, set after every update
	SDO_KEY		// required, not updatable, set once before creation
} ;

enum SpecFmt {
	SDF_NORMAL,	// no hint given
	SDF_LEFT,  	// left half only
	SDF_RIGHT,	// right half only; if follows LEFT goes on same line
	SDF_INDENT	// indented
} ;

class Spec {

    public:
			Spec();
			Spec( const char *encoded, const char *cmt );
			~Spec();

	// Using the Spec -- formatting and parsing forms

	StrBuf *	Format( SpecData *data )
			{ StrBuf *s = new StrBuf; Format( data, s ); return s; }

	void		Format( SpecData *data, StrBuf *result );
	void		Format( SpecData *data, StrDict *result );

	void		Parse( const char *buf, SpecData *data, Error *e, int valid );

	void		Parse( const char *buf, SpecData *data, Error *e )
			{ Parse( buf, data, e, 1 ); }
	void		ParseNoValid( const char *buf, SpecData *data, Error *e )
			{ Parse( buf, data, e, 0 ); }

	// Manipulating the Spec itself -- building and examining it

	SpecElem *	Add( const StrPtr &tag );
	SpecElem *	Get( int i );
	SpecElem *	Find( const StrPtr &tag, Error *e = 0 );
	SpecElem *	Find( int code, Error *e = 0 );
	int		Count();

	void		Decode( StrPtr *encoded, Error *e );
	void		Encode( StrBuf *encoded );

	const StrPtr *	GetComment() { return &comment; }
	void		SetComment( const StrPtr &c ) { comment = c; }

	SpecElem *	Add( char *t ) { return Add( StrRef( t ) ); }

    private:

	StrRef		comment;
	VarArray	*elems;
	StrBuf		decoderBuffer;
} ;

class SpecElem {

    public:

	// Type access

	int	IsDate() { return type == SDT_DATE; }
	int	IsSelect() { return type == SDT_SELECT; }
	int	IsText() { return type == SDT_TEXT 
				|| type == SDT_BULK; }
	int	IsList() { return type == SDT_WLIST 
				|| type == SDT_LLIST; }
	int	IsWords() { return type == SDT_WORD 
				|| type == SDT_WLIST
				|| type == SDT_SELECT; }
	int	IsSingle() { return type == SDT_WORD 
				|| type == SDT_SELECT
				|| type == SDT_LINE 
				|| type == SDT_DATE; }

	int	CheckValue( StrBuf &value );

	// Opt access

	int	IsRequired() { return opt == SDO_REQUIRED
				|| opt == SDO_KEY; }

	int	IsReadOnly() { return opt == SDO_ONCE
				|| opt == SDO_ALWAYS
				|| opt == SDO_KEY; }

	// Fmt access

	SpecFmt		GetFmt() { return fmt; }
	int		GetSeq() { return seq; }

	// Type building -- so jobspec can create a spec

	const char *	FmtOpt();
	const char *	FmtType();
	const char *	FmtFmt();
	void		SetSeq( int s ) { seq = s; }
	void 		SetOpt( const char *optName, Error *e );
	void 		SetFmt( const char *fmtName, Error *e );
	void		SetType( const char *s, Error *e );

	int		Compare( const SpecElem &other );

    public: // only to SpecData's subclasses

	SpecType	type;		// how it is formatted
	StrBuf		tag;		// name of the field
	StrBuf		preset;		// default preset value
	StrBuf		values;		// what values can be had
	int		code;		// what it's use it
	StrBuf		subCode;	// user's code
	char		nWords;		// how many words on the line
	short		maxLength;	// advisory
	SpecOpt		opt;		// how field is updated

    private:
    friend class Spec;

	void 		Decode( StrRef *s, Error *e );
	void		Encode( StrBuf *s, int code );

	// gui hints

	SpecFmt		fmt;		// format code
	int		seq;		// display sequence number

	// reference back to Get(index)

	int		index;		
} ;

class SpecWords : public StrBuf
{
    public:
	int 	Split();
	void 	Join( int wc );
	const char *wv[ SpecWordsMax + 1 ];
} ;

class SpecData {

    public:
			virtual ~SpecData() {}
	// Extract data from or build data into user's data structure.
	// Spec::Format() calls Get(); Spec::Parse() calls Set().

	// One of the two sets of Get/Set must be replaced in the subclass.

	// This interface assumes whole lines.  Its default implementation 
	// calls the word-oriented Get/Set and Joins/Splits them into
	// whole lines.

	virtual StrPtr *GetLine( SpecElem *sd, int x, const char **cmt );
	virtual void	SetLine( SpecElem *sd, int x, const StrPtr *val,
				Error *e );

	// This interface has words-oriented lines split apart.
	// The const version casts and calls the non-const version,
	// for compatibility.

	// The non-const one has a bogus default implementation.

	virtual int 	Get( SpecElem *sd, int x, const char **wv, const char **cmt );
	virtual void	Set( SpecElem *sd, int x, const char **wv, Error *e );

	virtual int 	Get( SpecElem *sd, int x, char **wv, char **cmt );
	virtual void	Set( SpecElem *sd, int x, char **wv, Error *e );

    protected:

	SpecWords	tVal;

} ;

class SpecDataTable : public SpecData {

    public:
			SpecDataTable();
			virtual ~SpecDataTable();

	virtual StrPtr *GetLine( SpecElem *sd, int x, const char **cmt );
	virtual void	SetLine( SpecElem *sd, int x, const StrPtr *val,
				Error *e );

	StrDict *	Dict() { return table; }

    private:

	StrDict		*table;

} ;

#endif /* _spec_h_ */
