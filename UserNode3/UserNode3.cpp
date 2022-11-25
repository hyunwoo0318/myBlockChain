#include"pch.h"
#include"UserNode.h"

int main()
{
	UserNode* userNode3 = new UserNode(3);
	PipeName pipeInfo;
	
	//임의의 tx를 주기적으로 생성함
	Transaction* tx = new Transaction();

	//F0과 통신
	while(1)
	{
		LPCSTR pipeName = pipeInfo.U3F0;		
		if (UserToFullClient(pipeName, tx))
			break;
	}


	
}