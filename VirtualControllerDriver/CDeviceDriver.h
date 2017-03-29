#pragma once

#include <openvr_driver.h>
#include <string>


class CDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRControllerComponent
{
public:

	static const vr::EVRButtonId k_EButton_Button1 = (vr::EVRButtonId) 7;
	static const vr::EVRButtonId k_EButton_Button2 = (vr::EVRButtonId) 8;
	static const vr::EVRButtonId k_EButton_Button3 = (vr::EVRButtonId) 9;
	static const vr::EVRButtonId k_EButton_Button4 = vr::k_EButton_ApplicationMenu;
	static const vr::EVRButtonId k_EButton_Bumper = vr::k_EButton_Grip; 

	CDeviceDriver();
	virtual ~CDeviceDriver();

	// ITrackedDeviceServerDriver
	virtual vr::EVRInitError Activate(uint32_t unObjectId);
	virtual void Deactivate();
	virtual void EnterStandby();
	void *GetComponent(const char *pchComponentNameAndVersion);
	virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);

	virtual vr::DriverPose_t GetPose();

	// IVRControllerComponent
	virtual vr::VRControllerState_t GetControllerState();
	virtual bool TriggerHapticPulse(uint32_t unAxisId, uint16_t usPulseDurationMicroseconds);
	
	void RunFrame();
	std::string GetSerialNumber() const { return m_sSerialNumber; }

private:
	vr::TrackedDeviceIndex_t m_unObjectId;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;
};
