#include"pch.h"
#include"UserNode.h"
#include"IPC.h"

int main()
{
	UserNode* userNode0 = new UserNode(0);
	userNode0->setEcKey(PRIV0);

	PipeName pipeInfo;

	//최초의 물건 생성 트랜잭션
	char modelNo1[] = "modelNo1 UserNode0";
	char modelNo2[] = "modelNo2 UserNode0";
	char modelNo3[] = "modelNo3 UserNode0";

	char others1[] = "others1 UserNode0";
	char others2[] = "others2 UserNode0";
	char others3[] = "others3 UserNode0";

	Transaction* t1 = userNode0->createProductTX(modelNo1, 10000, others1);
	Transaction* t2 = userNode0->createProductTX(modelNo1, 10000, others2);
	Transaction* t3 = userNode0->createProductTX(modelNo1, 10000, others3);

	//F1과 통신
	TCHAR sendBuf[BUFSIZE];
	LPCSTR pipeName = pipeInfo.U0F1;
	t1->serialize(sendBuf);

	for (int i = 0; i < 10; i++)
	{
		if (UserToFullClient(pipeName, sendBuf))
			cout << "send success!  " << i;
		Sleep(500);
	}
}