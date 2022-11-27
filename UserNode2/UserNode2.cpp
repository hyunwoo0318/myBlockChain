#include"pch.h"
#include"UserNode.h"
#include"IPC.h"
#pragma warning(disable : 4996)

int main()
{
	UserNode* userNode2 = new UserNode(2);
	userNode2->setEcKey(PRIV2);
	PipeName pipeInfo;

	//최초의 물건 생성 트랜잭션
	char modelNo1[] = "modelNo1 userNode2";
	char modelNo2[] = "modelNo2 userNode2";
	char modelNo3[] = "modelNo3 userNode2";

	char others1[] = "others1 userNode2";
	char others2[] = "others2 userNode2";
	char others3[] = "others3 userNode2";

	Transaction* t1 = userNode2->createProductTX(modelNo1, 10000, others1);
	Transaction* t2 = userNode2->createProductTX(modelNo1, 10000, others2);
	Transaction* t3 = userNode2->createProductTX(modelNo1, 10000, others3);

	//F4과 통신
	TCHAR sendBuf[BUFSIZE];
	LPCSTR pipeName = pipeInfo.U2F4;
	t1->serialize(sendBuf);

	for (int i = 0; i < 10; i++)
	{
		if (UserToFullClient(pipeName, sendBuf))
			cout << "send success!  " << i;
		Sleep(500);
	}
	while (1) {}



}