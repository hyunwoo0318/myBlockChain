#include"pch.h"
#include"FullNode.h"

//F0은 U3, F1, F4와 연결
int main()
{	
	PipeName pInfo;
	FullNode* fullNode0 = new FullNode(0);
	
		
	//U3과 연결
	LPCSTR pipeName = pInfo.U3F0;
	//Transaction* tx = UserToFullServer(pipeName);

	//F1과 연결
	{
		LPCSTR pipeName = pInfo.F0F1;
		HANDLE hF0F1 = connectPipe(pipeName);

		//받은 tx나 block을 넘겨줘야함.
	}
	
	
	//mining을 해야함.
	//멀티쓰레드 -> mining에 성공한 block을 받은경우 mining을 종료하고
	//그 뒤로 다시 실행해야함...  
	fullNode0->mining();
	
	/*DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);*/

}

