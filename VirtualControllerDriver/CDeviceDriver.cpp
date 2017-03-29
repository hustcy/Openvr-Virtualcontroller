#include "CDeviceDriver.h"
#include "driverlog.h"
#include <sstream>
#include <windows.h>
#include "SmStruct.h"

using namespace vr;

inline HmdQuaternion_t HmdQuaternion_Init(double w, double x, double y, double z)
{
	HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}

static const char * const k_pch_Section = "driver_virtualcontroller";
static const char * const k_pch_SerialNumber_String = "serialNumber";
static const char * const k_pch_ModelNumber_String = "modelNumber";

CDeviceDriver::CDeviceDriver() : m_unObjectId(vr::k_unTrackedDeviceIndexInvalid)
{
	m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;

	m_sSerialNumber = "you-set-a-sn-here";
	m_sModelNumber = "Visual Controller";

	EVRSettingsError e1, e2;
	vr::VRSettings()->SetString(k_pch_Section, k_pch_SerialNumber_String, m_sSerialNumber.c_str(), &e1 );
	vr::VRSettings()->SetString(k_pch_Section, k_pch_ModelNumber_String, m_sModelNumber.c_str(), &e2 );	
}

CDeviceDriver::~CDeviceDriver()
{	
}

EVRInitError CDeviceDriver::Activate(uint32_t unObjectId)
{
	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);

	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_ModelNumber_String, m_sModelNumber.c_str());
	DebugDriverLog("CDeviceDriver::Activate> ObjId:%x Container:%lld\n", unObjectId, m_ulPropertyContainer);
	return VRInitError_None;
}

void CDeviceDriver::Deactivate()
{
	DebugDriverLog("CDeviceDriver::Deactivate> .\n");
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
}

void CDeviceDriver::EnterStandby()
{
}

void *CDeviceDriver::GetComponent(const char *pchComponentNameAndVersion)
{
	DebugDriverLog("CDeviceDriver::GetComponent> %s \n", pchComponentNameAndVersion);

	if (!_stricmp(pchComponentNameAndVersion, vr::IVRControllerComponent_Version))
	{
		return (vr::IVRControllerComponent*)this;
	}

	return NULL;
}

void CDeviceDriver::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
	if (unResponseBufferSize >= 1)
		pchResponseBuffer[0] = 0;
}

VRControllerState_t CDeviceDriver::GetControllerState()
{
	DebugDriverLog("CDeviceDriver::GetControllerState> .\n");
	return vr::VRControllerState_t();
}

bool CDeviceDriver::TriggerHapticPulse(uint32_t unAxisId, uint16_t usPulseDurationMicroseconds)
{
	DebugDriverLog("CDeviceDriver::TriggerHapticPulse> .\n");
	return false;
}

DriverPose_t CDeviceDriver::GetPose()
{
	static float x, y, z, rx, ry, rz;

	static bool bDirection = true;
	static double dwY = 0.0;

	DriverPose_t pose = { 0 };

	pose.poseIsValid = true;
	pose.result = TrackingResult_Running_OK;
	pose.deviceIsConnected = true;

	pose.qWorldFromDriverRotation = HmdQuaternion_Init(1, 0, 0, 0);
	pose.qDriverFromHeadRotation = HmdQuaternion_Init(1, 0, 0, 0);

	pose.poseTimeOffset = 0.0f;// 0.016f;

	if ( GetPos(x,y,z,rx,ry,rz) )
	{ 
		pose.qRotation = HmdQuaternion_Init(1, rx, ry, rz);
		pose.vecPosition[0] = x;
		pose.vecPosition[1] = y;
		pose.vecPosition[2] = z;
	}
	else
	{ 
		pose.qRotation = HmdQuaternion_Init(1, 0, 0, 0);
		pose.vecPosition[0] = 0;
		pose.vecPosition[1] = 0;
		pose.vecPosition[2] = 0;
	}
		
	return pose;
}


void CDeviceDriver::RunFrame()
{
	// In a real driver, this should happen from some pose tracking thread.
	// The RunFrame interval is unspecified and can be very irregular if some other
	// driver blocks it for some periodic task.
	if (m_unObjectId != vr::k_unTrackedDeviceIndexInvalid)
	{
		vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, GetPose(), sizeof(DriverPose_t));
	}
}