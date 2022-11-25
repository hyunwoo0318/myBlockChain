#include"pch.h"
#include"FullNode.h"

int main()
{	
	FullNode* fullNode0 = new FullNode(0);
	TCHAR recvBuf[BUFSIZE];
	string s = "\\\\.\\pipe\\UTOF";
	LPCSTR pipeName = (LPCSTR)s.c_str();

	//연결 노드와의 통신 연결
	while (1)
	{		
		HANDLE hU0F0 = makePipe(pipeName);
		if (openPipe(hU0F0))
		{
			readFile(hU0F0,recvBuf);
			break;
		}
		//UserNode로 부터는 tx만 받으므로 따로 타입을 체크할 필요없음			
	}
	Product* p = new Product();
	p->deSerialize(recvBuf);
	cout << p->getDate() << " " << p->getIdentifier() << " "
		<< p->getModelNo();

	//mining을 해야함.
	//멀티쓰레드 -> mining에 성공한 block을 받은경우 mining을 종료하고
	//그 뒤로 다시 실행해야함...  
	fullNode0->mining();
	
	/*DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);*/

}

