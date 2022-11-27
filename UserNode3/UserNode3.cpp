#include"pch.h"
#include"UserNode.h"
#include"IPC.h"


int main()
{
	UserNode* userNode3 = new UserNode(3);
	userNode3->setEcKey(PRIV3);
	PipeName pipeInfo;
	
	//최초의 물건 생성 트랜잭션
	char modelNo1[] = "modelNo1 UserNode3";
	char modelNo2[] = "modelNo2 UserNode3";
	char modelNo3[] = "modelNo3 UserNode3";

	char others1[] = "others1 UserNode3";
	char others2[] = "others2 UserNode3";
	char others3[] = "others3 UserNode3";

	Transaction* t1 = userNode3->createProductTX(modelNo1, 10000, others1);
	Transaction* t2 = userNode3->createProductTX(modelNo1, 10000, others2);
	Transaction* t3 = userNode3->createProductTX(modelNo1, 10000, others3);

	//F0과 통신
	TCHAR sendBuf[BUFSIZE];
	LPCSTR pipeName = pipeInfo.U3F0;
	t1->serialize(sendBuf);

	for (int i = 0; i < 10; i++)
	{
		if (UserToFullClient(pipeName, sendBuf))
			cout << "send success!  " << i;
		Sleep(500);
	}


	
}