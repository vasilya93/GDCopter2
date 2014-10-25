#include "CommManager.h"

CommManager::CommManager()
{
	_commLog = new Log();
	_serialComm.CommLog = _commLog; //FileIO ���� ����� ��������� ����� � ����� ���������� � CommLog.
	_serialComm.AttachHandlerHost(this);
}

CommManager::~CommManager()
{
	//delete _commLog;
}

void CommManager::SerialBytesReceivedHandler(char* bytes, unsigned int bytesNum)
{
	_packetMaker.PushReceived(bytes, bytesNum);
	while(_packetMaker.IsMessageReady())
	{
		_packetMaker.ProcessRecMessage();
	}
}