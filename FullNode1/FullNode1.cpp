#include"pch.h"
#include"FullNode.h"
#include"IPC.h"

//F1는 U0, F0, F2, F4, F3과 연결
int main()
{	
	PipeName pInfo;
	FullNode* fullNode1 = new FullNode(1);

	//U0과 연결
	TCHAR recvBuf[BUFSIZE];
	TCHAR sendBuf[BUFSIZE];

	
	Transaction* tx = new Transaction();
	LPCSTR pipeName = pInfo.U0F1;

	while (1)
	{
		if (UserToFullServer(pipeName, recvBuf))
		{
			cout << "read success";
			tx->deserialize(recvBuf);
			fullNode1->addTx(*tx);
		}
		else
		{
			cout << "read fail! ";
			break;
		}
	}

	//F2와의 연결
}