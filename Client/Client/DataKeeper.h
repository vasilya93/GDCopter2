#include <stdint.h>

#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#define DATAKEEPER_ACCELX_RENEWED 0x01
#define DATAKEEPER_ACCELY_RENEWED 0x02
#define DATAKEEPER_ACCELZ_RENEWED 0x04
#define DATAKEEPER_ANGSPEEDX_RENEWED 0x08
#define DATAKEEPER_ANGSPEEDY_RENEWED 0x10
#define DATAKEEPER_ANGSPEEDZ_RENEWED 0x20

class DataKeeper {
	uint16_t _accelX;
	uint16_t _accelY;
	uint16_t _accelZ;
	
	uint16_t _angspeedX;
	uint16_t _angspeedY;
	uint16_t _angspeedZ;

	unsigned int _renewState;

	void _checkPrintAccel();
	void _checkPrintAngSpeed();
public:
	DataKeeper();
	~DataKeeper();

	void setAccelX(uint16_t accelX);
	void setAccelY(uint16_t accelY);
	void setAccelZ(uint16_t accelZ);

	void setAngSpeedX(uint16_t angspeedX);
	void setAngSpeedY(uint16_t angspeedY);
	void setAngSpeedZ(uint16_t angspeedZ);
};

#endif