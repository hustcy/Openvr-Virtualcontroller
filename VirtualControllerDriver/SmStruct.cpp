#include "stdafx.h"

#include "SmStruct.h"
#include "NameShareMem.h"

KNameShareMem			g_SmCapture;
PSmCapture				g_pSmData;

bool InitSm()
{
	bool bRet = false;
	if (!g_SmCapture.Create(def_VV_SMem_Name, sizeof(SmCapture)))
	{
		return bRet;
	}

	g_pSmData = (PSmCapture)g_SmCapture.GetBuf();

	if (ERROR_ALREADY_EXISTS == g_SmCapture.GetCreateError())
	{
		//g_pSmData->bAddDevice = FALSE;
		if (g_pSmData->uVersion != def_VV_SMEM_CAP_VERSION)
		{	

		}
	}
	else
	{
		ZeroMemory(g_pSmData, sizeof(SmCapture));
		g_pSmData->uVersion = def_VV_SMEM_CAP_VERSION;
		g_pSmData->bAddDevice = FALSE;		
	}
	bRet = true;

	return bRet;
}

bool GetPos(float& x, float& y, float& z, float& rx, float& ry, float& rz)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		x = g_pSmData->x;
		y = g_pSmData->y;
		z = g_pSmData->z;
		rx = g_pSmData->rx;
		ry = g_pSmData->ry;
		rz = g_pSmData->rz;
		return true;
	}
	return false;
}

bool NeedAddDevice()
{
	if (g_pSmData )
	{
		return g_pSmData->bAddDevice;
	}
	return false;
}

bool SetAddDevice(bool b)
{
	if (g_pSmData)
	{
		g_pSmData->bAddDevice = b;
		return true;
	}
	return false;
}

bool SetPosX(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->x = i;
		return true;
	}
	return false;
}
bool SetPosY(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->y = i;
		return true;
	}
	return false;
}
bool SetPosZ(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->z = i;
		return true;
	}
	return false;
}
bool SetPosRX(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->rx = i;
		return true;
	}
	return false;
}
bool SetPosRY(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->ry = i;
		return true;
	}
	return false;
}
bool SetPosRZ(float i)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->rz = i;
		return true;
	}
	return false;
}

bool SetPos(float x, float y, float z, float rx, float ry, float rz)
{
	if (g_pSmData && g_pSmData->bAddDevice)
	{
		g_pSmData->x = x;
		g_pSmData->y = y;
		g_pSmData->z = z;
		g_pSmData->rx = rx;
		g_pSmData->ry = ry;
		g_pSmData->rz = rz;
		return true;
	}
	return false;
}