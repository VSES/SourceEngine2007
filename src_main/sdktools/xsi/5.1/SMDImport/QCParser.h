// QCParser.h: interface for the QCParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCPARSER_H__F4E9E2E7_49F0_43CE_B727_C1E3B5646D29__INCLUDED_)
#define AFX_QCPARSER_H__F4E9E2E7_49F0_43CE_B727_C1E3B5646D29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCArray.h>

class QCParser  
{
public:
	QCParser();
	virtual ~QCParser();

	bool	Import ( char *in_szFilename );
	char*	FixFilenames(char *in_szFilename, bool noext = false);

	CSIBCArray<XSI::CString>	m_filelist;
	CSIBCArray<XSI::CString>	m_includelist;

};

#endif // !defined(AFX_QCPARSER_H__F4E9E2E7_49F0_43CE_B727_C1E3B5646D29__INCLUDED_)
