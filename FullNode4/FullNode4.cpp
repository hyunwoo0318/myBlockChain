#include"pch.h"
#include"FullNode.h"

//F4는  U2, F0, F1, F2, F3, F5과 연결
//F4는 모든 FullNode와 연결이 되어 있으므로 다른 FullNode와의 서버역할을 함.
int main()
{
	PipeName pInfo;
	FullNode* fullNode4 = new FullNode(4);

	//U0과 연결
	LPCSTR pipeName = pInfo.U2F4;
	Transaction* tx = UserToFullServer(pipeName);

	{
		TCHAR sendBuf[BUFSIZE];
		TCHAR recvBuf[BUFSIZE]
	}

}