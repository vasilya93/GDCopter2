#include <stdio.h>
#include "DataKeeper.h"

DataKeeper::DataKeeper() {
	_accelX = 0;
	_accelY = 0;
	_accelZ = 0;

	_angspeedX = 0;
	_angspeedX = 0;
	_angspeedX = 0;

	_renewState = 0;
}

DataKeeper::~DataKeeper() {
}

void DataKeeper::_checkPrintAccel() {
	if ((_renewState & DATAKEEPER_ACCELX_RENEWED) &&
		(_renewState & DATAKEEPER_ACCELY_RENEWED) &&
		(_renewState & DATAKEEPER_ACCELZ_RENEWED)) {

			_renewState &= ~(DATAKEEPER_ACCELX_RENEWED |
			                 DATAKEEPER_ACCELY_RENEWED |
							 DATAKEEPER_ACCELZ_RENEWED);
			printf("accelx: %7hd ", _accelX);
			printf("accely: %7hd ", _accelY);
			printf("accelz: %7hd\n", _accelZ);
	}
}

void DataKeeper::_checkPrintAngSpeed() {
	if ((_renewState & DATAKEEPER_ANGSPEEDX_RENEWED) &&
		(_renewState & DATAKEEPER_ANGSPEEDY_RENEWED) &&
		(_renewState & DATAKEEPER_ANGSPEEDZ_RENEWED)) {

			_renewState &= ~(DATAKEEPER_ANGSPEEDX_RENEWED |
			                 DATAKEEPER_ANGSPEEDY_RENEWED |
							 DATAKEEPER_ANGSPEEDZ_RENEWED);
			//printf("angspeedx: %7hd ", _angspeedX);
			//printf("angspeedy: %7hd ", _angspeedY);
			//printf("angspeedz: %7hd\n", _angspeedZ);
	}
}

void DataKeeper::setAccelX(uint16_t accelX) {
	_accelX = accelX;
	_renewState |= DATAKEEPER_ACCELX_RENEWED;
	_checkPrintAccel();
}

void DataKeeper::setAccelY(uint16_t accelY) {
	_accelY = accelY;
	_renewState |= DATAKEEPER_ACCELY_RENEWED;
	_checkPrintAccel();
}

void DataKeeper::setAccelZ(uint16_t accelZ) {
	_accelZ = accelZ;
	_renewState |= DATAKEEPER_ACCELZ_RENEWED;
	_checkPrintAccel();
}

void DataKeeper::setAngSpeedX(uint16_t angspeedX) {
	_angspeedX = angspeedX;
	_renewState |= DATAKEEPER_ANGSPEEDX_RENEWED;
	_checkPrintAngSpeed();
}

void DataKeeper::setAngSpeedY(uint16_t angspeedY) {
	_angspeedY = angspeedY;
	_renewState |= DATAKEEPER_ANGSPEEDY_RENEWED;
	_checkPrintAngSpeed();
}

void DataKeeper::setAngSpeedZ(uint16_t angspeedZ) {
	_angspeedZ = angspeedZ;
	_renewState |= DATAKEEPER_ANGSPEEDZ_RENEWED;
	_checkPrintAngSpeed();
}
