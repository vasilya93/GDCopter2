#include <string.h>
#include "DataKeeper.h"

#ifndef PACKETMAKER_H
#define PACKETMAKER_H

#define PAKMAK_BUFSIZE 500
#define PAKMAK_MESSIZE 5

#define MSNR_MT_BYTE 1
#define MSNR_MT_WORD 2
#define MSNR_MT_DWRD 3

#define MSNR_DD_MASK 0xFC
#define MSNR_DD_NONE 0x04
#define MSNR_DD_ACCELX 0x08
#define MSNR_DD_ACCELY 0x0C
#define MSNR_DD_ACCELZ 0x10
#define MSNR_DD_WHOAMI 0x14
#define MSNR_DD_PWRMGMT1 0x18
#define MSNR_DD_ANGSPEEDX 0x1C
#define MSNR_DD_ANGSPEEDY 0x20
#define MSNR_DD_ANGSPEEDZ 0x24
#define MSNR_DD_GYROXOFF 0x28
#define MSNR_DD_GYROYOFF 0x2C
#define MSNR_DD_GYROZOFF 0x30
#define MSNR_DD_ACCELXOFF 0x34
#define MSNR_DD_ACCELYOFF 0x38
#define MSNR_DD_ACCELZOFF 0x3C

#define I2C_MSG_STRTST 1
#define I2C_MSG_SBSND 2
#define I2C_MSG_ADSND 3
#define I2C_MSG_TRNSMTR 4
#define I2C_MSG_NACKST 5
#define I2C_MSG_RCVR 6
#define I2C_MSG_TRSCPL 7
#define I2C_MSG_TXE 8
#define I2C_MSG_RXNE 9
#define I2C_MSG_RXNE0 10
#define I2C_MSG_RXNE1 11
#define I2C_MSG_AF 12
#define I2C_MSG_STRTFL 13
#define I2C_MSG_ERR 14

#define MS5611_MSG_NODATA 31
#define MS5611_MSG_SENST1REQ 32
#define MS5611_MSG_SENST1 33
#define MS5611_MSG_OFFT1REQ 34
#define MS5611_MSG_OFFT1 35

#define MPU6050_MSG_WHOAMI 36
#define MPU6050_MSG_GETNEXTINIT 37
#define MPU6050_MSG_PROCOPRES 38
#define MPU6050_MSG_GETNEXTREG 39
#define MPU6050_MSG_ACCELX 40
#define MPU6050_MSG_ACCELY 41
#define MPU6050_MSG_ACCELZ 42
#define MPU6050_MSG_PWR1 43
#define MPU6050_MSG_PWR1_BAD 44
#define MPU6050_MSG_PWR1_GOOD 45
#define MPU6050_MSG_INIT_COMP 46

typedef union _32ToBytes
{
  float RealNum;
  unsigned int Integer;
  unsigned char Bytes[4];
}_32ToBytes;

class PacketMaker
{
	//Receive buffer details
	char _recBuf[PAKMAK_BUFSIZE];
	unsigned long _recFirstIn;
	unsigned long _recLastIn;
	unsigned long _recLeftFree;
	unsigned long _recBytesNum;
	bool _isMessageReady;
	DataKeeper _dataKeeper;

	//Common
	_32ToBytes _converter;
	unsigned int packet_size;

	//Funcs
	void _processPackageType(char* message);
	char* _decodeMessage(char);
	void _processDataDescription(char messageHeader, unsigned int data);

public:
	PacketMaker();
	~PacketMaker();

	//Access funcs
	bool IsMessageReady(){return _isMessageReady;}

	//Funcs
	void ClearBuf();
	bool PushReceived(char*, unsigned long);
	void ProcessRecMessage();
};

#endif