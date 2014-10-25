#include "PacketMaker.h"
#include "string.h"
#include "stdio.h"

PacketMaker::PacketMaker()
{
	ClearBuf();

	packet_size = 5; //1 for 1-byte packets
}

PacketMaker::~PacketMaker()
{
}

void PacketMaker::ClearBuf()
{
	_recFirstIn = 0;
	_recLastIn = 0;
	_recLeftFree = PAKMAK_BUFSIZE;
	_recBytesNum = 0;
	_isMessageReady = false;
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

	if (_recBytesNum >= packet_size)
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

	char *message = new char[packet_size];
	unsigned long leftToEnd = PAKMAK_BUFSIZE - _recFirstIn;
	if (leftToEnd >= packet_size)
	{
		memcpy_s(message, packet_size, &(_recBuf[_recFirstIn]), packet_size);
		_recFirstIn += packet_size;
		if (leftToEnd == packet_size)
		{
			_recFirstIn = 0;
		}
	}
	else
	{
		memcpy_s(message, packet_size, &(_recBuf[_recFirstIn]), leftToEnd);
		memcpy_s(&(message[leftToEnd]), PAKMAK_BUFSIZE - leftToEnd, _recBuf, PAKMAK_BUFSIZE - leftToEnd);
		_recFirstIn = PAKMAK_BUFSIZE - leftToEnd;
	}

	_recLeftFree += packet_size;
	_recBytesNum -= packet_size;

	if (_recBytesNum < packet_size)
	{
		_isMessageReady = false;
	}

	size_t textSize = 100;
	char* messageText = new char[textSize];

	if (packet_size == 1) {
		sprintf_s(messageText, textSize, "Message: %s\r\n", _decodeMessage(*message));
	} else {
		_translateMessage(message, messageText, textSize);
	}

	delete message;
	return messageText;

	//вызывающий эту функцию должен удалить messageText;
}

//-----------------------------Private functions -----------------------------

void PacketMaker::_translateMessage(char* message, char *messageText, size_t textSize)
{
	switch (message[0] & ~MSNR_DD_MASK) {
	case MSNR_MT_BYTE:
		sprintf_s(messageText, textSize, "Message: %s\r\n", _decodeMessage(message[4]));
		break;
	case MSNR_MT_WORD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = 0;
		_converter.Bytes[3] = 0;
		sprintf_s(messageText, textSize, "%s: %u\r\n", _get_data_description(message[0]), _converter.Integer);
		break;
	case MSNR_MT_DWRD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = message[2];
		_converter.Bytes[3] = message[1];
		sprintf_s(messageText, textSize, "%s: %u\r\n", _get_data_description(message[0]), _converter.Integer);
		break;
	default:
		sprintf_s(messageText, textSize, "Message header is not defined\r\n");
		break;
	}
}

char* PacketMaker::_decodeMessage(char code)
{
	switch (code)
	{
	case I2C_MSG_STRTST:
		return "I2C start set";
	case I2C_MSG_SBSND:
		return "I2C start-bit sent";
	case I2C_MSG_ADSND:
		return "I2C address sent";
	case I2C_MSG_TRNSMTR:
		return "I2C transmitter";
	case I2C_MSG_NACKST:
		return "I2C ACK reset";
	case I2C_MSG_RCVR:
		return "I2C receiver";
	case I2C_MSG_TRSCPL:
		return "I2C transmission complete";
	case I2C_MSG_TXE:
		return "I2C TX empty";
	case I2C_MSG_RXNE:
		return "I2C RX not empty";
	case I2C_MSG_RXNE0:
		return "I2C last byte received";
	case I2C_MSG_RXNE1:
		return "I2C second last byte received";
	case I2C_MSG_AF:
		return "I2C acknowledgement failure";
	case I2C_MSG_STRTFL:
		return "I2C start failure";
	case I2C_MSG_ERR:
		return "I2C unhandled error";
	case MS5611_MSG_NODATA:
		return "MS5611 next init operation NODATA";
	case MS5611_MSG_SENST1REQ:
		return "MS5611 next init operation SENST1REQ";
	case MS5611_MSG_SENST1:
		return "MS5611 next init operation SENST1";
	case MS5611_MSG_OFFT1REQ:
		return "MS5611 next init operation OFFT1REQ";
	case MS5611_MSG_OFFT1:
		return "MS5611 next init operation OFFT1";
	case MPU6050_MSG_WHOAMI:
		return "MPU6050 who am i received";
	case MPU6050_MSG_GETNEXTINIT:
		return "MPU6050 get next init operation was called";
	case MPU6050_MSG_PROCOPRES:
		return "MPU6050 process operation result was called";
	case MPU6050_MSG_GETNEXTREG:
		return "MPU6050 get next reg was called";
	case MPU6050_MSG_ACCELX:
		return "MPU6050 acceleration x received";
	case MPU6050_MSG_ACCELY:
		return "MPU6050 acceleration y received";
	case MPU6050_MSG_ACCELZ:
		return "MPU6050 acceleration z received";
	case MPU6050_MSG_PWR1:
		return "MPU6050 undefined data from PWR1 received";
	case MPU6050_MSG_PWR1_GOOD:
		return "MPU6050 good data from PWR1 received";
	case MPU6050_MSG_PWR1_BAD:
		return "MPU6050 bad data from PWR1 received";
	case MPU6050_MSG_INIT_COMP:
		return "MPU6050 initialization is completed";
	default:
		return "Unknown code";
		break;
	}
}

char* PacketMaker::_get_data_description(char message_header)
{
	message_header = message_header & MSNR_DD_MASK;
	switch (message_header) {
	case MSNR_DD_ACCELX:
		return "Acceleration x";
	case MSNR_DD_ACCELY:
		return "Acceleration y";
	case MSNR_DD_ACCELZ:
		return "Acceleration z";
	case MSNR_DD_WHOAMI:
		return "who am i value";
	case MSNR_DD_PWRMGMT1:
		return "PWRMGMT1 value";
	default:
		return "Unknown data";
	}
}