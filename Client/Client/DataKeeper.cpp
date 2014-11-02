#include <stdio.h>
#include "DataKeeper.h"

DataKeeper::DataKeeper() {
	Clear();
}

DataKeeper::~DataKeeper() {
}

void DataKeeper::_checkPrintAccel() {
	if ((_renewState & DATAKEEPER_ACCELX_RENEWED) &&
		(_renewState & DATAKEEPER_ACCELY_RENEWED) &&
		(_renewState & DATAKEEPER_ACCELZ_RENEWED)) {

			_renewState &= ~DATAKEEPER_ACCEL_RNWMASK;
			printf("accelx: %7hd ", _accelX);
			printf("accely: %7hd ", _accelY);
			printf("accelz: %7hd\n", _accelZ);
	}
}

void DataKeeper::_checkPrintAngSpeed() {
	if ((_renewState & DATAKEEPER_ANGSPEEDX_RENEWED) &&
		(_renewState & DATAKEEPER_ANGSPEEDY_RENEWED) &&
		(_renewState & DATAKEEPER_ANGSPEEDZ_RENEWED)) {

			_renewState &= ~DATAKEEPER_ANGSPEED_RNWMASK;
			//printf("angspeedx: %7hd ", _angspeedX);
			//printf("angspeedy: %7hd ", _angspeedY);
			//printf("angspeedz: %7hd\n", _angspeedZ);
	}
}

void DataKeeper::_checkPrintDCM()
{
	if ((_renewState & DATAKEEPER_DCM11_RENEWED) &&
		(_renewState & DATAKEEPER_DCM12_RENEWED) &&
		(_renewState & DATAKEEPER_DCM13_RENEWED) &&
		(_renewState & DATAKEEPER_DCM21_RENEWED) &&
		(_renewState & DATAKEEPER_DCM22_RENEWED) &&
		(_renewState & DATAKEEPER_DCM23_RENEWED) &&
		(_renewState & DATAKEEPER_DCM31_RENEWED) &&
		(_renewState & DATAKEEPER_DCM32_RENEWED) &&
		(_renewState & DATAKEEPER_DCM33_RENEWED)) {

		_renewState &= ~DATAKEEPER_DCM_RNWMASK;
		printf("%6.4f %6.4f %6.4f\n", _DCM[0][0], _DCM[0][1], _DCM[0][2]);
		printf("%6.4f %6.4f %6.4f\n", _DCM[1][0], _DCM[1][1], _DCM[1][2]);
		printf("%6.4f %6.4f %6.4f\n", _DCM[2][0], _DCM[2][1], _DCM[2][2]);
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

void DataKeeper::setDCM11(float value)
{
	_DCM[0][0] = value;
	_renewState |= DATAKEEPER_DCM11_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM12(float value)
{
	_DCM[0][1] = value;
	_renewState |= DATAKEEPER_DCM12_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM13(float value)
{
	_DCM[0][2] = value;
	_renewState |= DATAKEEPER_DCM13_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM21(float value)
{
	_DCM[1][0] = value;
	_renewState |= DATAKEEPER_DCM21_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM22(float value)
{
	_DCM[1][1] = value;
	_renewState |= DATAKEEPER_DCM22_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM23(float value)
{
	_DCM[1][2] = value;
	_renewState |= DATAKEEPER_DCM23_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM31(float value)
{
	_DCM[2][0] = value;
	_renewState |= DATAKEEPER_DCM31_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM32(float value)
{
	_DCM[2][1] = value;
	_renewState |= DATAKEEPER_DCM32_RENEWED;
	_checkPrintDCM();
}

void DataKeeper::setDCM33(float value)
{
	_DCM[2][2] = value;
	_renewState |= DATAKEEPER_DCM33_RENEWED;
	_checkPrintDCM();
}


void DataKeeper::Clear()
{
	_accelX = 0;
	_accelY = 0;
	_accelZ = 0;

	_angspeedX = 0;
	_angspeedX = 0;
	_angspeedX = 0;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			_DCM[i][j] = 0;
		}
	}

	_renewState = 0;
}
