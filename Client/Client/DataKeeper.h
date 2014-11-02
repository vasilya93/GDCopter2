#include <stdint.h>

#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#define DATAKEEPER_ACCELX_RENEWED 0x0001
#define DATAKEEPER_ACCELY_RENEWED 0x0002
#define DATAKEEPER_ACCELZ_RENEWED 0x0004
#define DATAKEEPER_ACCEL_RNWMASK (DATAKEEPER_ACCELX_RENEWED | DATAKEEPER_ACCELY_RENEWED | DATAKEEPER_ACCELZ_RENEWED)
#define DATAKEEPER_ANGSPEEDX_RENEWED 0x0008
#define DATAKEEPER_ANGSPEEDY_RENEWED 0x0010
#define DATAKEEPER_ANGSPEEDZ_RENEWED 0x0020
#define DATAKEEPER_ANGSPEED_RNWMASK (DATAKEEPER_ANGSPEEDX_RENEWED | DATAKEEPER_ANGSPEEDY_RENEWED | DATAKEEPER_ANGSPEEDZ_RENEWED)

#define DATAKEEPER_DCM11_RENEWED 0x0040
#define DATAKEEPER_DCM12_RENEWED 0x0080
#define DATAKEEPER_DCM13_RENEWED 0x0100
#define DATAKEEPER_DCM21_RENEWED 0x0200
#define DATAKEEPER_DCM22_RENEWED 0x0400
#define DATAKEEPER_DCM23_RENEWED 0x0800
#define DATAKEEPER_DCM31_RENEWED 0x1000
#define DATAKEEPER_DCM32_RENEWED 0x2000
#define DATAKEEPER_DCM33_RENEWED 0x4000
#define DATAKEEPER_DCM_RNWMASK 0x7FC0

class DataKeeper {
	uint16_t _accelX;
	uint16_t _accelY;
	uint16_t _accelZ;
	
	uint16_t _angspeedX;
	uint16_t _angspeedY;
	uint16_t _angspeedZ;

	float _DCM[3][3];

	unsigned int _renewState;

	void _checkPrintAccel();
	void _checkPrintAngSpeed();
	void _checkPrintDCM();
public:
	DataKeeper();
	~DataKeeper();

	void setAccelX(uint16_t accelX);
	void setAccelY(uint16_t accelY);
	void setAccelZ(uint16_t accelZ);

	void setAngSpeedX(uint16_t angspeedX);
	void setAngSpeedY(uint16_t angspeedY);
	void setAngSpeedZ(uint16_t angspeedZ);

	void setDCM11(float value);
	void setDCM12(float value);
	void setDCM13(float value);
	void setDCM21(float value);
	void setDCM22(float value);
	void setDCM23(float value);
	void setDCM31(float value);
	void setDCM32(float value);
	void setDCM33(float value);

	void Clear();
};

#endif