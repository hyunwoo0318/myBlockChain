#include"data.h"

class FullNode
{
private:
    int nodeNum;
    BlockChain blockChain;
    vector<Transaction> txPool;
public:
    FullNode(int nodeNum)
    {
        this->nodeNum = nodeNum;
    }
    //Block mining();
    bool validateTX(Transaction tx)
    {
        {
            //1) 판매하려는 물품의 최종 소유자가 tx의 input과 같은가?
            EC_KEY* sender = tx.getInput();
            int identifier = tx.getProduct().getIdentifier();
            if (this->blockChain.findProductOwner(identifier) != sender)
                return false;

            //2) immutable 필드값에 변화가 없는가?
            Product txProduct = tx.getProduct();
            Product product = this->blockChain.findProduct(identifier);
            if (!(product == txProduct))
                return false;

            //3)서명 검증
            EC_KEY* pubKey = tx.getInput();
            string digest = tx.getHashTx();
            const char* c = digest.c_str();
            const ECDSA_SIG* sig = tx.getSig();
            int ret = ECDSA_do_verify((unsigned char*)c, 32, sig, pubKey);
            if (ret != 1)
                return false;

            //valid한 TX로 판명
            //TODO :  다른 fullNode들에게 넘기기

            //자신의 txPool에 tx추가.
            txPool.push_back(tx);
            return true;
        }
    }
};

//TODO List
//1. full node는 계속 block과 TX를 받음.
//2. valid한 tx와 채굴 성공한 block은 넘겨줌.
//3. userNode로는 아무것도 보내지않음
//4. FullNode, userNode끼리 주어진 topology대로 연결.
