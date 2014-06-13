#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#define COMMMAN_MAXHANDNUM 5

#include "SerialComm.h"
#include "PacketMaker.h"

class CommManagerSubscribable
{
public:
	CommManagerSubscribable(){}
	~CommManagerSubscribable(){}

	virtual void MessageReadyHandler(char*) = 0;
};

class CommManager : private SerialCommSubscribable
{
	//Linked objects
	SerialComm _serialComm;
	Log* _commLog;
	PacketMaker _packetMaker;

	//Handlers details
	unsigned int _handlersNum;
	CommManagerSubscribable* _handlerHosts[COMMMAN_MAXHANDNUM];

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
	void ClearBuf(){_packetMaker.ClearBuf();}

	//Funcs
	bool AttachHandlerHost(CommManagerSubscribable* newHost);
};

#endif