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
    void addTx(Transaction tx);
    void UserToFullServer(LPCSTR pipeName);
    
};
