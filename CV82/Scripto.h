// Scripto.h : Declaration of the CScripto

#pragma once
#include "resource.h"       // main symbols



#include "CV82_i.h"
#include "_IScriptoEvents_CP.h"

#include <v8.h>
#include <hash_map>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

typedef std::hash_map< std::string, IDispatch*> DLIST;
typedef std::hash_map< std::string, IDispatch*>::iterator DITER;

// CScripto

class ATL_NO_VTABLE CScripto :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CScripto, &CLSID_Scripto>,
	public IConnectionPointContainerImpl<CScripto>,
	public CProxy_IScriptoEvents<CScripto>,
	public IDispatchImpl<IScripto, &IID_IScripto, &LIBID_CV82Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:

	v8::Persistent<v8::Object> _console;
	v8::Persistent<v8::Context> _context;
	v8::Persistent<v8::ObjectTemplate > _wrapper;

	DLIST dispatch_list;

	CScripto()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SCRIPTO)


BEGIN_COM_MAP(CScripto)
	COM_INTERFACE_ENTRY(IScripto)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CScripto)
	CONNECTION_POINT_ENTRY(__uuidof(_IScriptoEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		ResetContext();

		for (DITER iter = dispatch_list.begin(); iter != dispatch_list.end(); iter++)
		{
			if (iter->second) (iter->second)->Release();
		}

		v8::V8::Dispose();
	}

public:

	STDMETHOD(SetDispatch)(IDispatch *Dispatch, BSTR* Name);
	STDMETHOD(ExecString)(BSTR* Script, BSTR* Result, VARIANT_BOOL* Success);

	void Alert(const v8::FunctionCallbackInfo<v8::Value>& args);
	void Confirm(const v8::FunctionCallbackInfo<v8::Value>& args);
	void LogMessage(const v8::FunctionCallbackInfo<v8::Value>& args);

	void EventCallback(const v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> func);

	void ResetContext();
	void InitContext();
	void Getter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
	void Setter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
	void Invoker(const v8::FunctionCallbackInfo<v8::Value>& args);
	v8::Local< v8::Object > WrapDispatch(v8::Isolate *isolate, IDispatch *pdisp);
	void SetRetVal(v8::Isolate* isolate, CComVariant &var, v8::ReturnValue< v8::Value > &retval);
	void Indexed_Getter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);

};

OBJECT_ENTRY_AUTO(__uuidof(Scripto), CScripto)