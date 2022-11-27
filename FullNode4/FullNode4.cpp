#include"pch.h"
#include"FullNode.h"
#include"IPC.h"

//F4는  U2, F0, F1, F2, F3, F5과 연결
//F4는 모든 FullNode와 연결이 되어 있으므로 다른 FullNode와의 서버역할을 함.
int main()
{
	PipeName pInfo;
	FullNode* fullNode4 = new FullNode(4);

	TCHAR recvBuf[BUFSIZE];
	TCHAR sendBuf[BUFSIZE];

	//U2와 연결	
	Transaction* tx = new Transaction();
	LPCSTR pipeName = pInfo.U2F4;

	while (1)
	{
		if (UserToFullServer(pipeName, recvBuf))
		{
			cout << "read success";
			tx->deserialize(recvBuf);
			fullNode4->addTx(*tx);
		}
		else
		{
			cout << "read fail! ";
			break;
		}
	}

	//F0과의 연결
	pipeName = pInfo.F0F4;
	HANDLE hF0 = makePipe(pipeName);



	while (1) {}
}