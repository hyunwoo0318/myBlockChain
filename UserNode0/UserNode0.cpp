#include"pch.h"
#include"UserNode.h"

int main()
{
	UserNode* userNode0 = new UserNode(0);
	
	//F0와의 통신 -> 클라의 역할
	
	string s = "\\\\.\\pipe\\UTOF";
	LPCSTR pipeName = (LPCSTR)s.c_str();
	
	HANDLE hUTOF = connectPipe(pipeName);
	char msg[] = "modelNumber";
	Product* p = new Product(time(nullptr), 123123, msg);
	TCHAR sendBuf[BUFSIZE];
	p->serialize(sendBuf);
	writeFile(hUTOF,sendBuf);

	CloseHandle(hUTOF);
}