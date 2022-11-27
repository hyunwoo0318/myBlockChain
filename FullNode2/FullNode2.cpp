#include"pch.h"
#include"FullNode.h"
#include"IPC.h"

//F2는 U1, F1, F4, F5과 연결
int main()
{	
	PipeName pInfo;
	FullNode* fullNode2 = new FullNode(2);

	TCHAR recvBuf[BUFSIZE];
	TCHAR recvBuf2[BUFSIZE];
	TCHAR sendBuf[BUFSIZE];

	//U1과 연결	
	Transaction* tx = new Transaction();
	LPCSTR pipeName = pInfo.U1F2;	
	LPCSTR pipeName2 = pInfo.U0F1;	
	
	
		thread t1(fullNode2->UserToFullServer(pipeName), pipeName, recvBuf);
		thread t2(UserToFullServer, pipeName2, recvBuf2);
		tx->deserialize(recvBuf);
		fullNode2->addTx(*tx);

		t1.join();
		t2.join();
	
	
	/*
	while (1)
	{
		if (UserToFullServer(pipeName, recvBuf))
		{
			cout << "read success";
			tx->deserialize(recvBuf);
			fullNode2->addTx(*tx);
		}
		else
		{
			cout << "read fail! ";
			break;
		}
	}	*/

	//F1와의 연결
	while (1)
	{
		cout << "main thread";
		Sleep(100);
	}
}
