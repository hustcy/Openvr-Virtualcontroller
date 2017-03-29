#include <windows.h>

#include "VVServer.h"
#include "driverlog.h"


//-----------------------------------------------------------------------------
CWatchdogDriver g_watchdogDriver;
CVVServerDriver g_serverDriver;

// The only export function
extern "C" __declspec(dllexport) void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName))
	{
		return &g_serverDriver;
	}
	if (0 == strcmp(IVRWatchdogProvider_Version, pInterfaceName))
	{
		return &g_watchdogDriver;
	}

	if (pReturnCode)
		*pReturnCode = VRInitError_Init_InterfaceNotFound;
	
#ifdef _DEBUG
	char log[MAX_PATH] = { 0 };
	_snprintf_s(log, MAX_PATH, MAX_PATH-1, "HmdDriverFactory> Interface:%s  Ret:%d\n", pInterfaceName, *pReturnCode);
	OutputDebugStringA(log);
	DebugDriverLog("HmdDriverFactory> Interface:%s  Ret:%d\n", pInterfaceName, *pReturnCode);
#endif
	return NULL;
}


BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
#ifdef _DEBUG
	char FileName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, FileName, sizeof(FileName));
	char log[MAX_PATH] = { 0 };
	_snprintf_s(log, MAX_PATH, MAX_PATH - 1, "DllMain> Called! Reason:%d Process:%s\n", fdwReason, FileName);
	OutputDebugStringA(log);
	DebugDriverLog("DllMain> Called! Reason:%d Process:%s\n",fdwReason,FileName);
#endif

	return TRUE;
}