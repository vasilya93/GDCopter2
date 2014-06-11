#include "stdio.h"
#include "tchar.h"
#include "string.h"
#include "SerialComm.h"

#define MAIN_ANSWER_SIZE 50

class ConsoleWriter : public SerialCommSubscribable
{
public:
	ConsoleWriter() : SerialCommSubscribable()
	{}

	~ConsoleWriter(){}

	void SerialBytesReceivedHandler(char* bytes, unsigned int bytesNum)
	{
		printf("Received: %s", bytes);
	}

	void SerialBaudrateChangedHandler(unsigned long baudrate)
	{
	}

	void SerialNothingReceivedHandler()
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SerialComm serialComm;
	SerialCommSubscribable* consoleWriter = new ConsoleWriter();
	serialComm.AttachHandlerHost(consoleWriter);

	char answer[MAIN_ANSWER_SIZE];
	wchar_t port[MAIN_ANSWER_SIZE];
	bool result = true;

	while(result)
	{
		scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
		if(!strcmp("exit", answer))
		{
			serialComm.Disconnect();
			result = false;
		}
		else if(!strcmp("connect", answer))
		{
			scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
			size_t convNum;
			mbstowcs_s(&convNum, port, MAIN_ANSWER_SIZE, answer, _TRUNCATE);
			unsigned int error;
			serialComm.Connect(port, 115200, error);
		}
		else if(!strcmp("disconnect", answer))
		{
			serialComm.Disconnect();
		}
		else if(!strcmp("write", answer))
		{
			scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
			bool isLineUsed = true;// isLineUsed заменить как-то
			unsigned long mesLen = strlen(answer);
			serialComm.Write(answer, mesLen, &isLineUsed); 
		}
		else if(!strcmp("log", answer))
		{
			serialComm.SaveLogToFile();
		}
		else if(!strcmp("logclr", answer))
		{
			serialComm.ClearLog();
		}
	}

	return 0;
}