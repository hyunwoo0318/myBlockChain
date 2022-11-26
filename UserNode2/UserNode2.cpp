#include"pch.h"
#include"UserNode.h"

EC_KEY* ECKEY2;
int main()
{
	UserNode* userNode2 = new UserNode(2);
	userNode2->setEcKey(PRIV2);
	PipeName pipeInfo;

	//임의의 tx를 주기적으로 생성함
	Transaction* tx = new Transaction();

	//F0과 통신
	while (1)
	{
		LPCSTR pipeName = pipeInfo.U2F4;
		/*if (UserToFullClient(pipeName, tx))
			break;*/
	}



}