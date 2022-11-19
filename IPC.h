#pragma once
#include"utilHeader.h"

//pipe의 정보를 저장할 map선언
extern map<string, pair<HANDLE, const TCHAR*>> pipeInfo;

pair<HANDLE, const TCHAR*> createOwnPipe(string node1, string node2);

string readDatas(HANDLE hPipe);

bool sendDatas(const TCHAR* pipeName, char* data, char dataType);

void connectNodes();
