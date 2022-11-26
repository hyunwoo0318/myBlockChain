#pragma once


void readFile(HANDLE handle, TCHAR* recvBuf);

HANDLE makePipe(LPCSTR pipeName);

bool openPipe(HANDLE handle);

HANDLE connectPipe(LPCSTR pipeName);

bool writeFile(HANDLE handle,TCHAR* pkt);

bool UserToFullClient(LPCSTR pipeName, TCHAR* sendTx);

//Transaction* UserToFullServer(LPCSTR pipeName);

void FullToFullClient(LPCSTR pipeName);

void FullToFUllServer(LPCSTR pipeName);

void MasterToFullClient(LPCSTR pipeName);

void MasterToFullServer(LPCSTR pipeName);
