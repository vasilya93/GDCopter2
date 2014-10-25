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
	unsigned int _accelX;
	unsigned int _accelY;
	unsigned int _accelZ;
	
	unsigned int _angspeedX;
	unsigned int _angspeedY;
	unsigned int _angspeedZ;

	unsigned int _renewState;

	void _checkPrintAccel();
	void _checkPrintAngSpeed();
public:
	DataKeeper();
	~DataKeeper();

	void setAccelX(unsigned int accelX);
	void setAccelY(unsigned int accelY);
	void setAccelZ(unsigned int accelZ);

	void setAngSpeedX(unsigned int angspeedX);
	void setAngSpeedY(unsigned int angspeedY);
	void setAngSpeedZ(unsigned int angspeedZ);
};

#endif