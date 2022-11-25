#pragma once

void readFile(HANDLE handle, TCHAR* recvBuf);

HANDLE makePipe(LPCSTR pipeName);

bool openPipe(HANDLE handle);

HANDLE connectPipe(LPCSTR pipeName);

bool writeFile(HANDLE handle,TCHAR* pkt);
