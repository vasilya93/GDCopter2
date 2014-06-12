#include "PacketMaker.h"
#include "string.h"
#include "stdio.h"

PacketMaker::PacketMaker()
{
	_recFirstIn = 0;
	_recLastIn = 0;
	_recLeftFree = PAKMAK_BUFSIZE;
	_recBytesNum = 0;
	_isMessageReady = false;
}

PacketMaker::~PacketMaker()
{
}

bool PacketMaker::PushReceived(char* bytes, unsigned long bytesNum)
{
	if (_recLeftFree < bytesNum)
	{
		return false;
	}
	unsigned long leftToEnd = PAKMAK_BUFSIZE - _recLastIn;
	if (leftToEnd >= bytesNum)
	{
		memcpy_s(&(_recBuf[_recLastIn]), leftToEnd, bytes, bytesNum);
		_recLastIn += bytesNum;
		if (leftToEnd == bytesNum)
		{
			_recLastIn = 0;
		}
	}
	else
	{
		memcpy_s(&(_recBuf[_recLastIn]), leftToEnd, bytes, leftToEnd);
		memcpy_s(_recBuf, PAKMAK_BUFSIZE, &(bytes[leftToEnd]), bytesNum - leftToEnd);
		_recLastIn = bytesNum - leftToEnd;
	}

	_recLeftFree -= bytesNum;
	_recBytesNum += bytesNum;

	if (_recBytesNum >= PAKMAK_MESSIZE)
	{
		_isMessageReady = true;
	}

	return true;
}

char* PacketMaker::GetRecMessage()
{
	if (!_isMessageReady)
	{
		return "Message is not ready!!!";
	}

	char message[PAKMAK_MESSIZE];
	unsigned long leftToEnd = PAKMAK_BUFSIZE - _recFirstIn;
	if (leftToEnd >= PAKMAK_MESSIZE)
	{
		memcpy_s(message, PAKMAK_MESSIZE, &(_recBuf[_recFirstIn]), PAKMAK_MESSIZE);
		_recFirstIn += PAKMAK_MESSIZE;
		if (leftToEnd == PAKMAK_MESSIZE)
		{
			_recFirstIn = 0;
		}
	}
	else
	{
		memcpy_s(message, PAKMAK_MESSIZE, &(_recBuf[_recFirstIn]), leftToEnd);
		memcpy_s(&(message[leftToEnd]), PAKMAK_BUFSIZE - leftToEnd, _recBuf, PAKMAK_BUFSIZE - leftToEnd);
		_recFirstIn = PAKMAK_BUFSIZE - leftToEnd;
	}

	_recLeftFree += PAKMAK_MESSIZE;
	_recBytesNum -= PAKMAK_MESSIZE;

	if (_recBytesNum < PAKMAK_MESSIZE)
	{
		_isMessageReady = false;
	}

	char* messageText = _translateMessage(message);

	return messageText;

	//вызывающий эту функцию должен удалить messageText;
}

//-----------------------------Private functions -----------------------------

char* PacketMaker::_translateMessage(char* message)
{
	size_t textSize = 50;
	char* messageText = new char[textSize];

	switch (message[0])
	{
	case MSNR_MT_BYTE:
		sprintf_s(messageText, textSize, "Message: %s\r\n", _decodeMessage(message[4]));
		break;
	case MSNR_MT_WORD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = 0;
		_converter.Bytes[3] = 0;
		sprintf_s(messageText, textSize, "Word: %u\r\n", _converter.Integer);
		break;
	case MSNR_MT_DWRD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = message[2];
		_converter.Bytes[3] = message[1];
		sprintf_s(messageText, textSize, "Double word: %u\r\n", _converter.Integer);
		break;
	default:
		sprintf_s(messageText, textSize, "Message header is not defined\r\n");
		break;
	}

	return messageText;
}

char* PacketMaker::_decodeMessage(char code)
{
	switch (code)
	{
	case I2C_MSG_STRTST:
		return "I2C start set";
	default:
		return "Unknown code";
		break;
	}
}