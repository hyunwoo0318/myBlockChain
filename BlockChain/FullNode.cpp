#include "pch.h"
#include "FullNode.h"
#include<limits.h>

FullNode::FullNode(int nodeNum)
{
    this->nodeNum = nodeNum;
    setKeyList();
}

Block FullNode::mining()
{
    //뒤에 연결을 시도할 마지막 블록을 가져옴
    Block lastBlock = this->blockChain.getLastBlock();

    //nonce를 제외한 헤더값 설정
    //blockNo 설정 -> (이전 블록의 blockNo) + 1
    long blockNo = lastBlock.getHeader().getBlockNo() + 1;
    //mrklTree를 만들고 root의 hash값을 얻어옴
    MerkleTree* mrklTree = new MerkleTree();
    mrklTree->makeMerkleTree(this->txPool);
    string mrklRootHash = mrklTree->getRootHash();

    //이전 블록의 hash값을 가지는 hash pointer설정
    string prevHash = lastBlock.getHashRes();
    HashPointer* hashPointer = new HashPointer(&lastBlock, prevHash);

    //nonce를 제외한 header값 설정
    Header* header = new Header(blockNo, *hashPointer, mrklRootHash);

    //Block을 생성
    Block* block = new Block(*header,*mrklTree);

   while(1)
   {
        bool success = false;
        uint32_t nonce = 0;
        //nonce를 변경해가며 hash값 찾기
        for (int i = 0; i <= UINT32_MAX; i++)
        {
            nonce = i;
            header->nonce = nonce;
            string hashRes = hashTX(header);
            if (stoul(hashRes, nullptr, 0) <= TARGET_NUM)
            {
                success = true;
                break;
            }
        }
        if (success == true)
            break;
        else
        {
            //nonce를 다 돌려봤는데도 채굴에 실패 -> mrklTreeRoot를 바꿔야함.
            mrklTree->makeMerkleTree(this->txPool);
            string mrklRootHash = mrklTree->getRootHash();
            header->setMrklRootHash(mrklRootHash);
        }
    }
   return *block;    
}

bool FullNode::validateTX(Transaction tx)
{
    {
        //1) �Ǹ��Ϸ��� ��ǰ�� ���� �����ڰ� tx�� input�� ������?
        char* sender = tx.getInput();
        int identifier = tx.getProduct().getIdentifier();
        if (this->blockChain.findProductOwner(identifier) != sender)
            return false;

        //2) immutable �ʵ尪�� ��ȭ�� ���°�?
        Product txProduct = tx.getProduct();
        Product product = this->blockChain.findProduct(identifier);
        if (!(product == txProduct))
            return false;

        //3)���� ����
        //char* pubKey = tx.getInput();
        //EC_KEY* ecKey = findKey(pubKey);
        ////TODO : 예외처리하기
        //string digest = tx.getHashTx();
        //const char* c = digest.c_str();
        //const ECDSA_SIG* sig = tx.getSig();
        //int ret = ECDSA_do_verify((unsigned char*)c, 32, sig, ecKey);
        //if (ret != 1)
        //    return false;

        //valid�� TX�� �Ǹ�
        //TODO :  �ٸ� fullNode�鿡�� �ѱ��

        //�ڽ��� txPool�� tx�߰�.
        txPool.push_back(tx);
        return true;
    }
}

EC_KEY* FullNode::findKey(char* pubKey)
{
    /*for (int i = 0; i < this->keyList.size(); i++)
    {
        EC_KEY* ec = (this->keyList)[0];
        const EC_POINT* p = EC_KEY_get0_public_key(ec);
        if (pubKey == const_cast<EC_POINT*>(p))
            return ec;        
    }*/
    return NULL;
}

void FullNode::setKeyList()
{
    //4개의 userNode의 key를 모두 가지고있음
    EC_KEY* ecKey = makeEcKey(PRIV0);
    (this->keyList).push_back(ecKey);

    ecKey = makeEcKey(PRIV1);
    (this->keyList).push_back(ecKey);

    ecKey = makeEcKey(PRIV2);
    (this->keyList).push_back(ecKey);

    ecKey = makeEcKey(PRIV3);
    (this->keyList).push_back(ecKey);
}
