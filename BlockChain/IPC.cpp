#include"IPC.h"
#include"pch.h"


//usernode가 fullNode에게 tx를 보냄.
bool UserToFullClient(LPCSTR pipeName, TCHAR* pkt)
{
	HANDLE hPipe;
	DWORD writeSize;
	DWORD mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;

	bool success = false;

	hPipe = CreateFileA(
			pipeName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
	);	
		
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			auto msg = GetLastError();
			cout << "error in create File " << msg;
			return false;
		}
		else if (WaitNamedPipeA(pipeName, 10000) == false)
		{
			cout << "error in connect ";
			return false;
		}
	}
	
	SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

	success = WriteFile(
		hPipe,
		pkt,
		1024 * sizeof(TCHAR),
		&writeSize,
		NULL);
	if (success = false)
	{
		cout << "write error! ";
		return false;
	}		
	else
	{
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
		return true;
	}
		
}

