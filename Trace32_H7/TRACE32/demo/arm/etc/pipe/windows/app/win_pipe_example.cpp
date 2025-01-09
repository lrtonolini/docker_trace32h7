// win_pipe_example.cpp : This application eiterh will mirror the output of a "TERM.GATE" window in
//                        TRACE32 to a windows command shell. It serves as an example how the
//                        terminal data can be exported/mirrored on the fly.
//                        Or it will read in the data sent to a "TERM.view" window in TRACE32, i.e.
//                        then it serves as an example how to receive data from an external
//                        application.

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>

bool ParseParameters(int argc, char* argv[]);

int main(int argc, char *argv[])
{
	static const DWORD bufferSize = 1024;
	static const DWORD dwPipeMode = PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE;
	static const DWORD inoutBufferSize = bufferSize * 16;
	HANDLE handle = CreateNamedPipe(TEXT("\\\\.\\pipe\\my_pipe"), PIPE_ACCESS_DUPLEX, dwPipeMode, 1, inoutBufferSize,
		inoutBufferSize, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	bool bWriteExample = ParseParameters(argc, argv);

	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("\nCannot open pipe 'my_pipe', exit application");
		return 0;
	}

	printf("**************************************************************************************\n");
	printf("*                                                                                    *\n");
	if (bWriteExample)
		printf("*     Pipe write example for TRACE32 using 'TERM.GATE with dhrystone' on Windows     *\n");
	else
		printf("*  Pipe read example for TRACE32 using 'TERM.view' (no CPU application)' on Windows  *\n");
	printf("*                                                                                    *\n");
	printf("**************************************************************************************\n\n\n");

	printf("\nWaiting for pipe ... ");
	while (!ConnectNamedPipe(handle, NULL));                // Wait for 'TERM.Pipe #1 \\.\pipe\my_pipe' in TRACE32 script to connect to pipe
	printf("pipe connected\n\n");

	if (bWriteExample)
	{
		DWORD nReadChars;
		char pBuffer[bufferSize];                           // Read buffer

		// This will loop as long as "TERM.GATE #1" is open
		while (ReadFile(handle, pBuffer, sizeof(pBuffer) - 1, &nReadChars, NULL))
		{
			pBuffer[nReadChars] = '\0';                     // String termination
			printf("%s", pBuffer);                          // String output
		}
	}
	else
	{
		DWORD nWriteChars;
		static const char pHeadString[] = "TRACE32 pipe read example\n";
		static const char pEndString[]  = "Terminal read example ended\n";

		WriteFile(handle, pHeadString, sizeof(pHeadString) - 1, &nWriteChars, NULL);

		for (unsigned int i = 0; i < 1024; i++)
		{
			char pOutString[40], len = 0;

			sprintf_s(pOutString, "  * Terminal iteration %i \n", i);

			if (!WriteFile(handle, pOutString, strlen(pOutString), &nWriteChars, NULL))
				break;

			Sleep(500);
		}
	}

	printf("\nPipe closed");                                // You get here with 'TERM.CLOSE #1' or 'TERM.RESet #1' for example
	DisconnectNamedPipe(handle);

    return 0;
}

// -------------------------------------------------------------------------------------------------
// This is a primitive parser, it is enough for our needs
bool ParseParameters(int argc, char* argv[])
{
	if (argc <= 1)
		return true;                                        // app name itself is the only argument -> default to write example (TERM.GATE + dhrystone)

	if (argv[1][0] == '-')
	{
		switch (argv[1][1])
		{
		case 'r':
			return false;
		case 'w':
		default:
			return true;
		}
	}
	else
		return true;
}

