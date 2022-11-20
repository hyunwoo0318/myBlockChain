#pragma once
#include"data.h"

class FullNode
{
private:
    int nodeNum;
    BlockChain blockChain;
    vector<Transaction> txPool;
public:
    FullNode(int nodeNum);
   
    //Block mining();
    bool validateTX(Transaction tx);
    
};

//TODO List
//1. full node는 계속 block과 TX를 받음.
//2. valid한 tx와 채굴 성공한 block은 넘겨줌.
//3. userNode로는 아무것도 보내지않음
//4. FullNode, userNode끼리 주어진 topology대로 연결.