#include"pch.h"
#include"FullNode.h"
#include"IPC.h"

//F0은 U3, F1, F4와 연결
int main()
{	
	PipeName pInfo;
	FullNode* fullNode0 = new FullNode(0);	
		
	//U3과 연결
	TCHAR recvBuf[BUFSIZE];
	TCHAR sendBuf[BUFSIZE];

	
	Transaction* tx = new Transaction();
	LPCSTR pipeName = pInfo.U3F0;

	while (1)
	{
		if (UserToFullServer(pipeName, recvBuf))
		{
			cout << "read success";
			tx->deserialize(recvBuf);
			fullNode0->addTx(*tx);
		}
		else
		{
			cout << "read fail! ";
			break;
		}
	}

	//F4과 연결
	{
		//F4와의 파이프를 관리하는 handle획득
		LPCSTR pipeName = pInfo.F0F1;
		HANDLE hF0 = connectPipe(pipeName);

		

		//받은 tx나 block을 넘겨줘야함.
	}
	
	
	

}

