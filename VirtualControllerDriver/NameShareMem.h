#pragma once
#include <Windows.h>

#define def_VV_SMem_Name TEXT("you-create-a-GUID-here")


class KNameShareMem
{
public:
	KNameShareMem();
	~KNameShareMem();

public:
	BOOL Create(LPCTSTR lpszName, size_t dwSize);
	BOOL Open(LPCTSTR lpszName);
	VOID Close();

	PVOID GetBuf() { return m_pBuf; };
	DWORD GetCreateError() { return m_dwCreateError; };

private:
	HANDLE m_hMapping;
	DWORD m_dwSize;
	PVOID m_pBuf;
	DWORD m_dwCreateError;
};


class CLowSA
{
public:
	CLowSA() :m_pTmpSD(NULL), m_bError(FALSE)
	{
		m_bError = CreateALowSA(&m_SA, &m_SD) ? FALSE : TRUE;
	}

	~CLowSA()
	{
		if (m_pTmpSD != NULL)
		{
			::LocalFree(m_pTmpSD);
			m_pTmpSD = NULL;
		}
	}

	LPSECURITY_ATTRIBUTES GetSA()
	{
		if (m_bError)
			return NULL;
		return &m_SA;
	}

	operator LPSECURITY_ATTRIBUTES ()
	{
		return GetSA();
	}

protected:
	BOOL CreateALowSA(SECURITY_ATTRIBUTES *pSA, SECURITY_DESCRIPTOR *pSecurityD);

private:
	BOOL m_bError;
	SECURITY_ATTRIBUTES m_SA;
	SECURITY_DESCRIPTOR m_SD;
	PSECURITY_DESCRIPTOR m_pTmpSD;
};