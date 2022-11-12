#pragma once
#include"data.h"

/*
FullNode

UserNode로부터 TX를 받으면 일단 검증을 한다.
검증에 성공하면 해당 TX를 포함하여 채굴을 시도한다.

각각의 FullNode(개별 proces)는 blockchain을 가지고있다.
*/

class FullNode 
{
private:
	BlockChain blockChain;
	vector<Transaction> txPool;
public:
	Block mining();
	bool validateTX(Transaction tx);
};

//TODO List
//1. full node는 계속 block과 TX를 받음.
//2. valid한 tx와 채굴 성공한 block은 넘겨줌.
//3. userNode로는 아무것도 보내지않음
//4. FullNode, userNode끼리 주어진 topology대로 연결.


