#pragma once
#include"data.h"
#define TARGET_NUM (0x00ffffff)

class FullNode
{
private:
    int nodeNum;
    BlockChain blockChain;
    vector<Transaction> txPool;
    vector<EC_KEY*> keyList;
public:
    FullNode(int nodeNum);
   
    Block mining();
    bool validateTX(Transaction tx);
    EC_KEY* findKey(char* pubKey);
    void setKeyList();
    
};

//TODO List
//1. full node�� ��� block�� TX�� ����.
//2. valid�� tx�� ä�� ������ block�� �Ѱ���.
//3. userNode�δ� �ƹ��͵� ����������
//4. FullNode, userNode���� �־��� topology��� ����.