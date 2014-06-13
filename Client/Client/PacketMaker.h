#ifndef PACKETMAKER_H
#define PACKETMAKER_H

#define PAKMAK_BUFSIZE 500
#define PAKMAK_MESSIZE 5

#define MSNR_MT_BYTE 1
#define MSNR_MT_WORD 2
#define MSNR_MT_DWRD 3

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

#define MS5611_MSG_NODATA 31
#define MS5611_MSG_SENST1REQ 32
#define MS5611_MSG_SENST1 33
#define MS5611_MSG_OFFT1REQ 34
#define MS5611_MSG_OFFT1 35

#include "string.h"

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

	//Common
	_32ToBytes _converter;

	//Funcs
	char* _translateMessage(char*);
	char* _decodeMessage(char);

public:
	PacketMaker();
	~PacketMaker();

	//Access funcs
	bool IsMessageReady(){return _isMessageReady;}

	//Funcs
	void ClearBuf();
	bool PushReceived(char*, unsigned long);
	char* GetRecMessage();
};

#endif