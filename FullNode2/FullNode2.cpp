#include"pch.h"
#include"FullNode.h"

//F2는 U1, F1, F4, F5과 연결
int main()
{
	PipeName pInfo;
	FullNode* fullNode2 = new FullNode(2);

	//U0과 연결
	LPCSTR pipeName = pInfo.U0F1;
	Transaction* tx = UserToFullServer(pipeName);
}