#include "stdio.h"
#include "tchar.h"
#include "string.h"
#include "SerialComm.h"
#include "CommManager.h"

#define MAIN_ANSWER_SIZE 50

class ConsoleWriter : public SerialCommSubscribable, public CommManagerSubscribable
{
public:
	ConsoleWriter() : SerialCommSubscribable(), CommManagerSubscribable()
	{}

	~ConsoleWriter(){}

	void SerialBytesReceivedHandler(char* bytes, unsigned int bytesNum)
	{
		printf("%s", bytes);
	}

	void SerialBaudrateChangedHandler(unsigned long baudrate)
	{
	}

	void SerialNothingReceivedHandler()
	{
	}

	void MessageReadyHandler(char* message)
	{
		printf("%s", message);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	CommManager commManager;
	ConsoleWriter consoleWriter;
	commManager.AttachHandlerHost(&consoleWriter);

	char answer[MAIN_ANSWER_SIZE];
	wchar_t port[MAIN_ANSWER_SIZE];
	bool result = true;

	while(result)
	{
		scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
		if(!strcmp("exit", answer))
		{
			commManager.Disconnect();
			result = false;
		}
		else if(!strcmp("connect", answer))
		{
			scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
			size_t convNum;
			mbstowcs_s(&convNum, port, MAIN_ANSWER_SIZE, answer, _TRUNCATE);
			unsigned int error;
			commManager.Connect(port, 115200, error);
		}
		else if(!strcmp("disconnect", answer))
		{
			commManager.Disconnect();
		}
		else if(!strcmp("write", answer))
		{
			scanf_s("%49s", answer, MAIN_ANSWER_SIZE);
			bool isLineUsed = true;// isLineUsed заменить как-то
			unsigned long mesLen = strlen(answer);
			commManager.Write(answer, mesLen, &isLineUsed); 
		}
		else if(!strcmp("log", answer))
		{
			commManager.SaveLogToFile();
		}
		else if(!strcmp("logclr", answer))
		{
			commManager.ClearLog();
		}
		else if(!strcmp("cls", answer))
		{
			system("cls");
		}
		else if(!strcmp("bufclr", answer))
		{
			commManager.ClearBuf();
		}
	}

	return 0;
}