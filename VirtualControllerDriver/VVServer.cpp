#include "VVServer.h"
#include "driverlog.h"
#include "SmStruct.h"

EVRInitError CVVServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());
	
	m_bEnableDriver = true;
	bool bSm = InitSm();
	if (bSm)
	{
		SetAddDevice(false);
	}
	DebugDriverLog("CVVServerDriver::Init> SM: %d\n", bSm);

	return VRInitError_None;
}

void CVVServerDriver::Cleanup()
{
	DebugDriverLog("CVVServerDriver::Cleanup> .");
	CleanupDriverLog();
}


void CVVServerDriver::RunFrame()
{
	if (NeedAddDevice() && !createdController) 
	{
		createdController = true;
		DebugDriverLog("CVVServerDriver::RunFrame> Adding virtual device!\n");

		if (!m_pDeviceDriver)
		{
			m_pDeviceDriver = new CDeviceDriver();
			vr::VRServerDriverHost()->TrackedDeviceAdded(m_pDeviceDriver->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pDeviceDriver);
		}
	}

	if (m_pDeviceDriver)
	{
		m_pDeviceDriver->RunFrame();
	}
}

/////////////////////////////////////////////////////////////////////////////
bool g_bExiting = false;

void WatchdogThreadFunction()
{
	while (!g_bExiting)
	{
		// on windows send the event when the Y key is pressed.
		if ((0x01 /*& GetAsyncKeyState('Y')*/) != 0)
		{
			// Y key was pressed. 
			vr::VRWatchdogHost()->WatchdogWakeUp();
		}
		std::this_thread::sleep_for(std::chrono::microseconds(5000));
	}
}

EVRInitError CWatchdogDriver::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_WATCHDOG_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());

	// Watchdog mode on Windows starts a thread that listens for the 'Y' key on the keyboard to 
	// be pressed. A real driver should wait for a system button event or something else from the 
	// the hardware that signals that the VR system should start up.
	g_bExiting = false;
	m_pWatchdogThread = new std::thread(WatchdogThreadFunction);
	if (!m_pWatchdogThread)
	{
		DriverLog("Unable to create watchdog thread\n");
		return VRInitError_Driver_Failed;
	}

	return VRInitError_None;
}


void CWatchdogDriver::Cleanup()
{
	g_bExiting = true;
	if (m_pWatchdogThread)
	{
		m_pWatchdogThread->join();
		delete m_pWatchdogThread;
		m_pWatchdogThread = nullptr;
	}

	CleanupDriverLog();
}

