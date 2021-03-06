#include "SerialComm.h"
#include "windows.h"
#include "assert.h"
#include "thread"

using namespace std;

//---------------------------------------Constructors --------------------------------------

SerialComm::SerialComm()
{
	_packetCounter = 0;

	_writeSync.hEvent = INVALID_HANDLE_VALUE;
	HandlersNum = 0;

	_firstTick = 0;
	_lastTick = 0;

	_isReadingContinued = false;
	ReadBufSize = READ_BUF_SIZE + 1;
	ReadBuf = new char[ReadBufSize];
	hPort = 0;
}

//---------------------------------------Destructors --------------------------------------

SerialComm::~SerialComm()
{
	Disconnect();
	delete ReadBuf;
}

//---------------------------------------Access funcs --------------------------------------


bool SerialComm::ResetTick(unsigned long* firstTick, unsigned long* lastTick)
{
	if (firstTick==NULL || lastTick==NULL)
	{
		return false;
	}
	*firstTick = _firstTick;
	*lastTick = _lastTick;
	_firstTick = 0;
	_lastTick = 0;
	return true;
}

//-----------------------------------------Functions ----------------------------------------

bool SerialComm::AttachHandlerHost(SerialCommSubscribable* newHost)
{
	if(HandlersNum > MAX_HANDLERS_NUMBER)
	{
		CommLog->AddMessage("SerialComm.AttachHandlerHost: handlers' pool is full.\r\n");
		return false;
	}
	HandlersHosts[HandlersNum++] = newHost;
	CommLog->AddMessage("SerialComm.AttachHandlerHost: handler is attached.\r\n");
	return true;
}

bool SerialComm::Connect(const wchar_t* port, int baudrate, unsigned int& errMessage)
{
	if(port == L"")
	{
		errMessage = COMM_EM_NOSERSEL;
		CommLog->AddMessage("SerialComm.Connect: port name is empty.\r\n");
		return false;
	}
	hPort = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hPort == INVALID_HANDLE_VALUE)
	{
		errMessage = COMM_EM_CNTOPNSER;
		CommLog->AddMessage("SerialComm.Connect: failed to create serial port file.\r\n");
		return false;
	}
	DCB dcb;
	ZeroMemory(&dcb, sizeof(DCB));
	BOOL bSuccess;
	bSuccess = GetCommState(hPort, &dcb);
	if(!bSuccess)
	{
		errMessage = COMM_EM_GTSTFL;
		CommLog->AddMessage("SerialComm.Connect: failed to get communication state.\r\n");
		return false;
	}
	dcb.BaudRate = (DWORD)baudrate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	bSuccess = SetCommState(hPort, &dcb);
	if(!bSuccess)
	{
		errMessage = COMM_EM_STSTFL;
		CommLog->AddMessage("SerialComm.Connect: failed to set communication state.\r\n");
		return false;
	}
	GetCommState(hPort, &dcb);
	COMMTIMEOUTS Timeouts = {1, 0, 100/*timeoutConstant*/, 0, 0};
	bSuccess = SetCommTimeouts(hPort, &Timeouts);
	if(!bSuccess)
	{
		errMessage = COMM_EM_STTMFL;
		CommLog->AddMessage("SerialComm.Connect: failed to set communication timeouts.\r\n");
		return false;
	}

	_isReadingContinued = true;

	std::thread readThread(&SerialComm::_readThread, std::ref(*this));
	readThread.detach();

	CommLog->AddMessage("SerialComm.Connect: connection is completed.\r\n");
	return true;
}

void SerialComm::Disconnect()
{//when receiving and sending will be asynchronous it will be necessary
	//to check port statuses (or overlapped, or events) to ensure it's not
	//in the middle of an operation
	if(hPort != 0)
	{
		_isReadingContinued = false;
		_readMutex.lock();
		_readMutex.unlock();
		_writeMutex.lock(); //to wait for the mutex to be released
		_writeMutex.unlock();
		CloseHandle(hPort);
		hPort = 0;
		CommLog->AddMessage("SerialComm.Disconnect: port is disconnected.\r\n");
		return;
	}

	CommLog->AddMessage("SerialComm.Disconnect: invalid port handler.\r\n");
	return;
}

bool SerialComm::Write(char* line, unsigned long lineSize, bool* isLineUsed) //� PacketMaker isLineUsed ������������ � ��� �������� �� ��, ��������� �� ������, ���������
{
	if(isLineUsed == NULL)
	{
		CommLog->AddMessage("SerialComm.Write: isLineUsed parameter is NULL.\r\n");
		return false;
	}
	if(*isLineUsed == false)
	{
		CommLog->AddMessage("SerialComm.Write: isLineUsed parameter is false.\r\n");
		return false;
	}
	_isLineUsed = isLineUsed;

	_writeMutex.lock();

	if(_writeSync.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_writeSync.hEvent);
	}
	ZeroMemory(&_writeSync, sizeof(OVERLAPPED));
	_writeSync.hEvent = CreateEvent(NULL, false, false, NULL);


	//����� �� ��������, � ����� ������ ��?
	if(!WriteFile(hPort, line, lineSize, NULL, &_writeSync))
	{
		//CommLog->AddMessage("SerialComm.Write: WriteFile returned zero.\r\n"); //is asynchronous and always return with zero
		if(GetLastError() != ERROR_IO_PENDING)
		{
			CommLog->AddMessage("SerialComm.Write: WriteFile critical error.\r\n");
			//unsigned long error = GetLastError();
			//wchar_t* errorMessage = new wchar_t[200];
			//wcscpy_s(errorMessage, 200, L"WriteFile:");
			//unsigned long length = wcslen(errorMessage);
			//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, errorMessage, 200, NULL);
			//MessageBox(NULL, errorMessage, L"Error", MB_OK);
			//delete errorMessage;
			_writeMutex.unlock();
			return false;
		}
	}

	_packetCounter++;
	_internalHigh = _writeSync.InternalHigh;

	std::thread writeThread(&SerialComm::_writeThread, std::ref(*this));
	writeThread.detach();

	_writeMutex.unlock();

	Sleep(0); //���� ����������� ������������ ������ �������� �������

	CommLog->AddMessage("SerialComm.Write: successfully completed.\r\n");
	return true;
}

//---------------------------------------Threads --------------------------------------

void SerialComm::_writeThread()
{
	_writeMutex.lock(); //����� ��������, ��� ������ ���������� � ������� ������� ������ ���� �����
	unsigned long charsWritten;
	if(!GetOverlappedResult(hPort, &_writeSync, &charsWritten, true))
	{
		/*unsigned long error = GetLastError();
		wchar_t* errorMessage = new wchar_t[200];
		wcscpy_s(errorMessage, 200, L"GetOverlapped:");
		unsigned long length = wcslen(errorMessage);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, &(errorMessage[length + 1]), 200-length, NULL);
		MessageBox(NULL, errorMessage, L"Error", MB_OK);
		delete errorMessage;*/
		CommLog->AddMessage("SerialComm._writeThread: GetOverlappedResult returned zero.\r\n");
	}

	*_isLineUsed = false;
	_writeMutex.unlock();

	CommLog->AddMessage("SerialComm._writeThread: write operation is accomplished.\r\n");
	return;
}

void SerialComm::_readThread()
{
	unsigned long bytesRead;
	BOOL isSuccess;
	unsigned long error;
	HANDLE hReadCompEvent = CreateEvent(NULL, false, false, NULL);
	OVERLAPPED sync = {0};
	sync.hEvent = hReadCompEvent;
	_readMutex.lock();
	while(_isReadingContinued)
	{
		isSuccess = ReadFile(hPort, ReadBuf, (ReadBufSize - 1), NULL, &sync); //����� ������ ��� GetOverlapped result
																			  //����������� ����� � ����
		if(!isSuccess)
		{
			//CommLog->AddMessage("SerialComm._readThread: ReadFile returned zero.\r\n"); //is asynchronous and always returns with zero
			error = GetLastError();
			if(error != ERROR_IO_PENDING)
			{
				CommLog->AddMessage("SerialComm._readThread: ReadFile critical fail.\r\n");
			}
		}
		isSuccess = GetOverlappedResult(hPort, &sync, &bytesRead, true);
		if(!isSuccess)
		{
			CommLog->AddMessage("SerialComm._readThread: GetOverlappedResult returned zero.\r\n");
			//return;
		}
		if(bytesRead > 0)
		{
			_lastTick = GetTickCount();
			CommLog->AddMessage("SerialComm._readThread: bytes received.\r\n");
			if(_firstTick == 0)
			{
				_firstTick = _lastTick;
			}
			ReadBuf[bytesRead] = '\0';
			for (unsigned int i = 0; i < HandlersNum; i++)
			{
				//����������� ������ ����������� ������, ����� ������������
				HandlersHosts[i]->SerialBytesReceivedHandler(ReadBuf, bytesRead);
			}
		}
		else
		{
			for (unsigned int i = 0; i < HandlersNum; i++)
			{
				HandlersHosts[i]->SerialNothingReceivedHandler();
			}
		}
	}
	_readMutex.unlock();
	return;
}