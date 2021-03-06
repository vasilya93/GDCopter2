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

	_dataKeeper.Clear();
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

void PacketMaker::ProcessRecMessage()
{
	if (!_isMessageReady) {
		return;
	}

	char *message = new char[packet_size];
	unsigned long leftToEnd = PAKMAK_BUFSIZE - _recFirstIn;
	if (leftToEnd >= packet_size) {
		memcpy_s(message, packet_size, &(_recBuf[_recFirstIn]), packet_size);
		_recFirstIn += packet_size;

		if (leftToEnd == packet_size) {
			_recFirstIn = 0;
		}
	} else {
		memcpy_s(message, packet_size, &(_recBuf[_recFirstIn]), leftToEnd);
		memcpy_s(&(message[leftToEnd]), PAKMAK_BUFSIZE - leftToEnd, _recBuf, PAKMAK_BUFSIZE - leftToEnd);
		_recFirstIn = PAKMAK_BUFSIZE - leftToEnd;
	}

	_recLeftFree += packet_size;
	_recBytesNum -= packet_size;

	if (_recBytesNum < packet_size)	{
		_isMessageReady = false;
	}

	if (packet_size == 1) {
		printf("Message: %s\r\n", _decodeMessage(*message));
	} else {
		_processPackageType(message);
	}

	delete message;
}

//-----------------------------Private functions -----------------------------

void PacketMaker::_processPackageType(char* message)
{
	switch (message[0] & ~MSNR_DD_MASK) {
	case MSNR_MT_BYTE:
		printf("Message: %s\r\n", _decodeMessage(message[4]));
		break;
	case MSNR_MT_WORD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = 0;
		_converter.Bytes[3] = 0;
		_processDataDescription(message[0]);
		break;
	case MSNR_MT_DWRD:
		_converter.Bytes[0] = message[4];
		_converter.Bytes[1] = message[3];
		_converter.Bytes[2] = message[2];
		_converter.Bytes[3] = message[1];
		_processDataDescription(message[0]);
		break;
	default:
		printf("ProcessPackageType: error, unknown package type %u!\n", message[0] & ~MSNR_DD_MASK);
		break;
	}
}

char* PacketMaker::_decodeMessage(char code)
{
	switch (code) {
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
	case SENSFUS_MSG_ACC_INSIDE:
		return "accelerometer data loaded into fusion alg";
	case SENSFUS_MSG_GYRO_INSIDE:
		return "gyro data loaded into fusion alg";
	default:
		return "Unknown code";
		break;
	}
}

void PacketMaker::_processDataDescription(char messageHeader)
{
	messageHeader = messageHeader & MSNR_DD_MASK;
	switch (messageHeader) {
	case MSNR_DD_ACCELX:
		_dataKeeper.setAccelX(_converter.Integer);
		break;
	case MSNR_DD_ACCELY:
		_dataKeeper.setAccelY(_converter.Integer);
		break;
	case MSNR_DD_ACCELZ:
		_dataKeeper.setAccelZ(_converter.Integer);
		break;
	case MSNR_DD_ANGSPEEDX:
		_dataKeeper.setAngSpeedX(_converter.Integer);
		break;
	case MSNR_DD_ANGSPEEDY:
		_dataKeeper.setAngSpeedY(_converter.Integer);
		break;
	case MSNR_DD_ANGSPEEDZ:
		_dataKeeper.setAngSpeedZ(_converter.Integer);
		break;
	case MSNR_DD_DCM11:
		_dataKeeper.setDCM11(_converter.RealNum);
		break;
	case MSNR_DD_DCM12:
		_dataKeeper.setDCM12(_converter.RealNum);
		break;
	case MSNR_DD_DCM13:
		_dataKeeper.setDCM13(_converter.RealNum);
		break;
	case MSNR_DD_DCM21:
		_dataKeeper.setDCM21(_converter.RealNum);
		break;
	case MSNR_DD_DCM22:
		_dataKeeper.setDCM22(_converter.RealNum);
		break;
	case MSNR_DD_DCM23:
		_dataKeeper.setDCM23(_converter.RealNum);
		break;
	case MSNR_DD_DCM31:
		_dataKeeper.setDCM31(_converter.RealNum);
		break;
	case MSNR_DD_DCM32:
		_dataKeeper.setDCM32(_converter.RealNum);
		break;
	case MSNR_DD_DCM33:
		_dataKeeper.setDCM33(_converter.RealNum);
		break;
	case MSNR_DD_GYROXOFF:
		printf("gyro x offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_GYROYOFF:
		printf("gyro y offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_GYROZOFF:
		printf("gyro z offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_ACCELXOFF:
		printf("accel x offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_ACCELYOFF:
		printf("accel y offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_ACCELZOFF:
		printf("accel z offset value: %hd\n", (int16_t) _converter.Integer);
		break;
	case MSNR_DD_WHOAMI:
		printf("who am i value: %u\n", _converter.Integer);
		break;
	case MSNR_DD_PWRMGMT1:
		printf("PWRMGMT1 value: %u\n", _converter.Integer);
		break;
	default:
		printf("ProcessDataDescription: error, unknown data description: %u\n",
		       messageHeader & MSNR_DD_MASK);
		printf("ProcessDataDescription: data: %u\n", _converter.Integer);
		break;
	}
}