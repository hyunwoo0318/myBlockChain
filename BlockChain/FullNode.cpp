#include "pch.h"
#include "FullNode.h"

FullNode::FullNode(int nodeNum)
{
    this->nodeNum = nodeNum;
}

bool FullNode::validateTX(Transaction tx)
{
    {
        //1) �Ǹ��Ϸ��� ��ǰ�� ���� �����ڰ� tx�� input�� ������?
        EC_KEY* sender = tx.getInput();
        int identifier = tx.getProduct().getIdentifier();
        if (this->blockChain.findProductOwner(identifier) != sender)
            return false;

        //2) immutable �ʵ尪�� ��ȭ�� ���°�?
        Product txProduct = tx.getProduct();
        Product product = this->blockChain.findProduct(identifier);
        if (!(product == txProduct))
            return false;

        //3)���� ����
        EC_KEY* pubKey = tx.getInput();
        string digest = tx.getHashTx();
        const char* c = digest.c_str();
        const ECDSA_SIG* sig = tx.getSig();
        int ret = ECDSA_do_verify((unsigned char*)c, 32, sig, pubKey);
        if (ret != 1)
            return false;

        //valid�� TX�� �Ǹ�
        //TODO :  �ٸ� fullNode�鿡�� �ѱ��

        //�ڽ��� txPool�� tx�߰�.
        txPool.push_back(tx);
        return true;
    }
}