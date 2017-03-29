#pragma once
#include <openvr_driver.h>
#include "CDeviceDriver.h"
#include <thread>

using namespace vr;


class CVVServerDriver : public IServerTrackedDeviceProvider
{
public:
	CVVServerDriver() : m_pDeviceDriver(NULL), m_bEnableDriver(false){}

	virtual EVRInitError Init(vr::IVRDriverContext *pDriverContext);
	virtual void Cleanup();
	virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame();

	virtual bool ShouldBlockStandbyMode() { return false; }
	virtual void EnterStandby() {}
	virtual void LeaveStandby() {}

private:
	CDeviceDriver			*m_pDeviceDriver;

	bool m_bEnableDriver;
	bool createdController = false;
};


class CWatchdogDriver : public IVRWatchdogProvider
{
public:
	CWatchdogDriver()
	{
		m_pWatchdogThread = nullptr;
	}

	virtual EVRInitError Init(vr::IVRDriverContext *pDriverContext);
	virtual void Cleanup();

private:
	std::thread *m_pWatchdogThread;
};