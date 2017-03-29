#include "stdafx.h"

#include "NameShareMem.h"
#include <Sddl.h>

// -------------------------------------------------------------------------
KNameShareMem::KNameShareMem()
{
	m_hMapping = NULL;
	m_dwSize = NULL;
	m_pBuf = NULL;
	m_dwCreateError = S_OK;
}

KNameShareMem::~KNameShareMem()
{
	Close();
}

BOOL KNameShareMem::Create(LPCTSTR lpszName, size_t dwSize)
{
	BOOL bRet = FALSE;

	CLowSA sa;
	m_hMapping = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		sa,
		PAGE_READWRITE,
		0, (DWORD)dwSize, 
		lpszName);
	m_dwCreateError = ::GetLastError();
	if (m_hMapping != NULL)
	{
		m_pBuf = ::MapViewOfFile(
			m_hMapping,
			FILE_MAP_WRITE | FILE_MAP_READ,
			0, 0, 0);

		if (m_pBuf != NULL)
		{
			bRet = TRUE;
		}
		else
		{
			m_dwSize = 0;
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			
		}
	}
	else
	{
		
	}

	return bRet;
}

BOOL KNameShareMem::Open(LPCTSTR lpszName)
{
	BOOL bRet = FALSE;
	m_hMapping = ::OpenFileMapping(
		FILE_MAP_READ | FILE_MAP_WRITE, 
		FALSE,
		lpszName);

	if (m_hMapping)
	{
		m_pBuf = ::MapViewOfFile(
			m_hMapping,
			FILE_MAP_WRITE | FILE_MAP_READ,
			0, 0, 0);

		if (m_pBuf)
		{
			bRet = TRUE;
		}
		else
		{
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
	}

	return bRet;
}

VOID KNameShareMem::Close()
{
	if (m_pBuf != NULL)
	{
		::UnmapViewOfFile(m_pBuf);
		m_dwSize = 0;
		m_pBuf = NULL;
	}
	if (m_hMapping != NULL)
	{
		::CloseHandle(m_hMapping);
		m_hMapping = NULL;
	}
}


// -------------------------------------------------------------------------
#define LOW_INTEGRITY_SDDL_SACL TEXT("S:(ML;;NW;;;LW)")

BOOL CLowSA::CreateALowSA(SECURITY_ATTRIBUTES *pSA, SECURITY_DESCRIPTOR *pSecurityD)
{
	if (m_pTmpSD != NULL)
	{
		::LocalFree(m_pTmpSD);
		m_pTmpSD = NULL;
	}

	BOOL bFuncRet = FALSE;
	PACL pSacl = NULL;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;

	pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
	pSA->bInheritHandle = FALSE;
	pSA->lpSecurityDescriptor = pSecurityD;
	do
	{
		bFuncRet = ::InitializeSecurityDescriptor(pSA->lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
		if (!bFuncRet)
		{
			break;
		}

#pragma warning(push)
#pragma warning(disable: 6248)
		bFuncRet = ::SetSecurityDescriptorDacl(pSA->lpSecurityDescriptor, TRUE, 0, FALSE);
#pragma warning(pop)
		if (!bFuncRet)
		{
			break;
		}

		bFuncRet = ::ConvertStringSecurityDescriptorToSecurityDescriptor(
			LOW_INTEGRITY_SDDL_SACL, SDDL_REVISION_1, &m_pTmpSD, NULL);	
		if (!bFuncRet)
		{
			bFuncRet = TRUE;
			break;
		}

		bFuncRet = ::GetSecurityDescriptorSacl(m_pTmpSD, &fSaclPresent, &pSacl, &fSaclDefaulted);
		if (!bFuncRet)
		{			
			break;
		}
		bFuncRet = ::SetSecurityDescriptorSacl(pSA->lpSecurityDescriptor, TRUE, pSacl, FALSE);
		if (!bFuncRet)
		{			
			break;
		}

		bFuncRet = TRUE;

	} while (0);

	return bFuncRet;
}
