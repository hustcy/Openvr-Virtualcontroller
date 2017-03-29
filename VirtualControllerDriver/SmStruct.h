#pragma once

#define def_VV_SMEM_CAP_VERSION	1

typedef struct _tagSmCapture
{
	unsigned uVersion;			// version
	bool    bAddDevice;
	float	x;
	float	y;
	float	z;
	float	rx;
	float	ry;
	float	rz;
}SmCapture, *PSmCapture;

bool InitSm();
bool GetPos(float& x, float& y, float& z, float& rx, float& ry, float& rz);
bool NeedAddDevice();

bool SetAddDevice(bool b);
bool SetPosX(float i);
bool SetPosY(float i);
bool SetPosZ(float i);
bool SetPosRX(float i);
bool SetPosRY(float i);
bool SetPosRZ(float i);
bool SetPos(float x, float y, float z, float rx, float ry, float rz);