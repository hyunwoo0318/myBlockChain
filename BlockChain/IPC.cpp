#include"IPC.h"
#include"pch.h"

void readFile(HANDLE handle, TCHAR* recvBuf)
{
	bool success = false;
	DWORD readSize;

	success = ReadFile(
		handle,
		recvBuf,
		BUFSIZE * sizeof(TCHAR),
		&readSize,
		NULL);

	if (!success || readSize == 0)
	{
		cout << "read Error!" << endl;		
	}

	FlushFileBuffers(handle);	
}

HANDLE makePipe(LPCSTR pipeName)
{
	HANDLE hPipe;
	hPipe = CreateNamedPipeA(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,
		BUFSIZE,
		BUFSIZE,
		20000,
		NULL);
	return hPipe;
}

bool openPipe(HANDLE handle)
{
	bool success = false;
	success = ConnectNamedPipe(handle, NULL);
	return success;
}

HANDLE connectPipe(LPCSTR pipeName)
{
	HANDLE hPipe;
	DWORD mode = PIPE_READMODE_BYTE | PIPE_WAIT;
	while (1)
	{
		hPipe = CreateFileA(
			pipeName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
		);

		if (hPipe != INVALID_HANDLE_VALUE)
			break;
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			cout << "error in create File ";
			break;
		}
		if (WaitNamedPipeA(pipeName, 20000) == false)
		{
			cout << "error in connect ";
			break;
		}
	}
	SetNamedPipeHandleState(
		hPipe,
		&mode,
		NULL, NULL);
	return hPipe;
}

bool writeFile(HANDLE handle, TCHAR* pkt)
{
	bool success = false;
	
	DWORD writeSize;
	success = WriteFile(
		handle,
		pkt,
		1024 * sizeof(TCHAR),
		&writeSize,
		NULL);
	if (success)
	{
		cout << "send message success" << endl;
		return true;
	}
	else
	{
		cout << "send message failed" << endl;
		return false;
	}
}

bool UserToFullClient(LPCSTR pipeName, Transaction* sendTx)
{
	TCHAR sendBuf[BUFSIZE];
	HANDLE h = connectPipe(pipeName);
	sendTx->serialize(sendBuf);
	return writeFile(h, sendBuf);	
}

Transaction* UserToFullServer(LPCSTR pipeName)
{
	TCHAR recvBuf[BUFSIZE];
	Transaction* tx = new Transaction();
	while (1)
	{		
		HANDLE hU3F0 = makePipe(pipeName);
		if (openPipe(hU3F0))
		{
			readFile(hU3F0, recvBuf);
			break;
		}
		//UserNode로 부터는 tx만 받으므로 따로 타입을 체크할 필요없음		
		tx->deserialize(recvBuf);
	}
	return tx;
}

void FullToFullClient(LPCSTR pipeName)
{

}

void FullToFUllServer(LPCSTR pipeName)
{
}

void MasterToFullClient(LPCSTR pipeName)
{
}

void MasterToFullServer(LPCSTR pipeName)
{
}
