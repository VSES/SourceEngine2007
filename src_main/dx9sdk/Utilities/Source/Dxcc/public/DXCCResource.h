#ifndef DXCCRESOURCE_H
#define DXCCRESOURCE_H

interface __declspec( uuid("75ebc313-afa7-4f1b-80f7-2b1256a28b29") ) IDXCCResource;
DEFINE_GUID( IID_IDXCCResource, 
			0x75ebc313, 
			0xafa7, 
			0x4f1b, 
			0x80, 0xf7, 0x2b, 0x12, 0x56, 0xa2, 0x8b, 0x29);

typedef interface	IDXCCResource				IDXCCResource					,*LPDXCCRESOURCE;


#undef INTERFACE
#define INTERFACE IDXCCResource
DECLARE_INTERFACE_(IDXCCResource, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_  REFIID riid,  void** ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	//get the fast access handle...used with the manager to find this resource
	STDMETHOD_(DXCCHANDLE, GetHandle)(THIS) PURE;

	//get the object's creations IID/UUID
	STDMETHOD_(REFIID, GetIID)(THIS) PURE;

	//Get the object it is attached to
	//this operates like QueryInterface
	//but fails if object is not attached with AttachObject
	STDMETHOD(GetObject)(THIS_ 
		 LPUNKNOWN* ppvObj) PURE;	//output a pointer to the interface

	STDMETHOD(QueryObject)(THIS_ 
		 REFIID riid,			//what would you like me to query for?
		 void** ppvObj) PURE;	//output a pointer to the interface

	//reattach an object to the resource 
	//so that the interface can be retrieved from the resource
	STDMETHOD(AttachObject)(THIS_ 
		 LPUNKNOWN pValidation) PURE; //you must supply the interface ptr in order to re-attach the object otherwise i dont know if it is deleted

	//detach an interface from the resource 
	//a detatched resource can reach ref count of 0
	//but you can no longer GetObject until you re-attach with AttachObject
	//return TRUE if ref count is zero
	//return FALSE if detached or ref count has not reached zero
	STDMETHOD_(BOOL, DetachObject)(THIS) PURE; 

	//can I use GetObject to retrieve this object
	STDMETHOD_(BOOL, Attached)(THIS) PURE;

	STDMETHOD_(BOOL, Equals)(THIS_ 
		 LPUNKNOWN pValidation) PURE; //you must supply the interface ptr in order to re-attach the object otherwise i dont know if it is deleted

	//what is my name?
	STDMETHOD_(LPCSTR, GetName)(THIS) PURE;

	//what is my name?
	STDMETHOD(SetName)(THIS_ 
		 LPCSTR name) PURE;

	//what resource did i originate from?
	STDMETHOD_(LPCSTR, GetResourcePath)(THIS) PURE;

	//what resource did i originate from?
	STDMETHOD(SetResourcePath)(THIS_ 
		 LPCSTR name) PURE;


	STDMETHOD(GetProperties)(THIS_ 
		LPDXCCPROPERTYBAG* pPropertyBag) PURE;
};

#endif