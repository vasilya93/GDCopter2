#include "CommManager.h"

CommManager::CommManager()
{
	_commLog = new Log();
	_serialComm.CommLog = _commLog; //FileIO тоже нужно создавать здесь и позже передавать в CommLog.
	_serialComm.AttachHandlerHost(this);

	_handlersNum = 0;
}

CommManager::~CommManager()
{
	//delete _commLog;
}

bool CommManager::AttachHandlerHost(CommManagerSubscribable* newHost)
{
	if(_handlersNum > COMMMAN_MAXHANDNUM)
	{
		_commLog->AddMessage("CommManager.AttachHandlerHost: handlers' pool is full.\r\n");
		return false;
	}
	_handlerHosts[_handlersNum++] = newHost;
	_commLog->AddMessage("CommManager.AttachHandlerHost: handler is attached.\r\n");
	return true;
}

void CommManager::SerialBytesReceivedHandler(char* bytes, unsigned int bytesNum)
{
	_packetMaker.PushReceived(bytes, bytesNum);
	while(_packetMaker.IsMessageReady())
	{
		char* messageText = _packetMaker.GetRecMessage();
		for (unsigned int i = 0; i < _handlersNum; i++)
		{
			_handlerHosts[i]->MessageReadyHandler(messageText);
		}
		delete messageText;
	}
}