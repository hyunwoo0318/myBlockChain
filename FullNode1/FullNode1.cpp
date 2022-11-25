#include"pch.h"
#include"FullNode.h"

//F1는 U0, F0, F2, F4, F3과 연결
int main()
{	
	PipeName pInfo;
	FullNode* fullNode1 = new FullNode(1);

	//U0과 연결
	LPCSTR pipeName = pInfo.U0F1;
	Transaction* tx = UserToFullServer(pipeName);	
}