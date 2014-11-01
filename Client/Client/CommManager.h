#include "SerialComm.h"
#include "PacketMaker.h"

#ifndef COMMMANAGER_H
#define COMMMANAGER_H

class CommManager : private SerialCommSubscribable
{
	//Linked objects
	SerialComm _serialComm;
	Log* _commLog;
	PacketMaker _packetMaker;

	//Functions
	void SerialBytesReceivedHandler(char*, unsigned int);
	void SerialBaudrateChangedHandler(unsigned long){}
	void SerialNothingReceivedHandler(){}

public:
	CommManager();
	~CommManager();

	//Access funcs
	bool Connect(const wchar_t* port, int baudrate, unsigned int& errMessage)
	{return _serialComm.Connect(port, baudrate, errMessage);}
	void Disconnect(){_serialComm.Disconnect();}
	bool Write(char* line, unsigned long lineSize, bool* isLineUsed)
	{return _serialComm.Write(line, lineSize, isLineUsed);}
	void SaveLogToFile(){_commLog->SaveToFile();}
	void ClearLog(){_commLog->Clear();}
	void ClearBuf();
};

#endif