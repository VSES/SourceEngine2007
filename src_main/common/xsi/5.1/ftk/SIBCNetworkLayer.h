#ifndef _SIBCNETWORKLAYER_H_
#define _SIBCNETWORKLAYER_H_

#include <SIBCUtil.h>
#include <SIBCArray.h>
#include "SINetworkMessages.h"

typedef SI_Error (*NETMSGPROVIDER) ( SINetMsg* , SI_Void* );

struct	SINetworkProviderInfo
{
	NETMSGPROVIDER	m_pFnc;
	SI_Int			m_iMsg;
	SI_Void*		m_pUserData;
};

class SIBCNetworkLayer
{
public:
	SIBCNetworkLayer();
	virtual ~SIBCNetworkLayer();

	virtual SI_Error	Initialize	( SI_Void * );
	virtual SI_Error	Shutdown	( SI_Void * );
	
	virtual SI_Error	RecvCmd		( SI_Void * );
	virtual SI_Error	PeekCmd		( SI_Void * );
	virtual SI_Error	SendCmd		( SI_Void * );

	virtual SI_Error	DispatchNetMessage	( SINetMsg*	in_pMsg );

	virtual	SI_Error	RegisterProvider	( SINetworkProviderInfo& );

	virtual SI_Error	NotifyCmdEnd		();

protected:

	CSIBCArray<SINetworkProviderInfo>		m_pProviderList;

	SI_Char				m_szHostName[128];

};

//
// The following must be defined seperatly on each plateform
//

SIBCNetworkLayer*	InitializeNetworkLayer	( SI_Void * );		
SI_Error			ShutdownNetworkLayer	( SIBCNetworkLayer* );

#endif
