#include "utils/FileDownloader.hpp"

#include <iostream>

#ifdef _WIN32
	#include <urlmon.h>
	#pragma comment(lib, "Urlmon.lib")
#endif

auto FileDownloader::download(	const std::string& url,
								const std::string& fileName ) -> bool
{
	#ifdef _WIN32
		
		class DownloadStatusCallback : public IBindStatusCallback
		{
		public:
			HRESULT _stdcall OnStartBinding( DWORD, IBinding* )						{ return E_NOTIMPL; }
			HRESULT _stdcall GetPriority( LONG* )									{ return E_NOTIMPL; }
			HRESULT _stdcall OnLowResource( DWORD )									{ return E_NOTIMPL; }
			HRESULT _stdcall OnStopBinding( HRESULT, LPCWSTR )						{ return E_NOTIMPL; }
			HRESULT _stdcall GetBindInfo( DWORD*, BINDINFO* )						{ return E_NOTIMPL; }
			HRESULT _stdcall OnDataAvailable(DWORD, DWORD, FORMATETC*, STGMEDIUM*)	{ return E_NOTIMPL; }
			HRESULT _stdcall OnObjectAvailable(	REFIID, IUnknown* )					{ return E_NOTIMPL; }
			HRESULT _stdcall QueryInterface( REFIID, void ** )						{ return E_NOTIMPL; }
			ULONG _stdcall AddRef()													{ return 0;	}
			ULONG _stdcall Release()												{ return 0; }

			HRESULT _stdcall OnProgress(ULONG	ulProgress,
										ULONG	ulProgressMax,
										ULONG	ulStatusCode,
										LPCWSTR szStatusText)
			{
				//std::cout << ulProgress << "\n";
				return S_OK;
			}
		};

		DownloadStatusCallback statusCallback;

		auto rval = URLDownloadToFile(	nullptr,
										url.data(),
										fileName.data(),
										0,
										&statusCallback );
		
		return rval == S_OK;
	#endif
}