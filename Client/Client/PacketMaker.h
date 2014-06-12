#ifndef PACKETMAKER_H
#define PACKETMAKER_H

#define PAKMAK_BUFSIZE 500
#define PAKMAK_MESSIZE 5

#define MSNR_MT_BYTE 1
#define MSNR_MT_WORD 2
#define MSNR_MT_DWRD 3

#define I2C_MSG_STRTST 1

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
	bool PushReceived(char*, unsigned long);
	char* GetRecMessage();
};

#endif