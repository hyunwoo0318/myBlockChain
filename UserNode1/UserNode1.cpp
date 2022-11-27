#include"pch.h"
#include"UserNode.h"
#include"IPC.h"

int main()
{
	Sleep(1000);
	UserNode* userNode1 = new UserNode(1);
	userNode1->setEcKey(PRIV1);
	PipeName pipeInfo;

	//최초의 물건 생성 트랜잭션
	char modelNo1[] = "modelNo1 UserNode1";
	char modelNo2[] = "modelNo2 UserNode1";
	char modelNo3[] = "modelNo3 UserNode1";

	char others1[] = "others1 UserNode1";
	char others2[] = "others2 UserNode2";
	char others3[] = "others3 UserNode3";

	Transaction* t1 = userNode1->createProductTX(modelNo1, 10000, others1);
	Transaction* t2 = userNode1->createProductTX(modelNo1, 10000, others2);
	Transaction* t3 = userNode1->createProductTX(modelNo1, 10000, others3);
	
	//F2과 통신
	TCHAR sendBuf[BUFSIZE];
	LPCSTR pipeName = pipeInfo.U1F2;
	t1->serialize(sendBuf);
	
	for (int i = 0; i < 10; i++)
	{
		if (UserToFullClient(pipeName, sendBuf))
			cout << "send success!  " << i;
		Sleep(1000);
	}
	
	while (1) {}

}