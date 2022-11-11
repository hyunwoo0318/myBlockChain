#include"FullNode.h"

//Block FullNode::mining()
//{
//    
//}

bool FullNode::validateTX(Transaction tx)
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
    const unsigned char* digest = tx.getHashTx();
    const ECDSA_SIG* sig = tx.getSig();
    int ret = ECDSA_do_verify(digest, 32, sig, pubKey);
    if (ret != 1)
        return false;

    //valid한 TX로 판명
    //TODO :  다른 fullNode들에게 넘기기

    //자신의 txPool에 tx추가.
    txPool.push_back(tx);
    return true;
}
