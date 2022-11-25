#include "pch.h"
#include "UserNode.h"


UserNode::UserNode(int nodeID)
{
	random_device rd;
	mt19937 gen(rd());
	this->identifier = gen();

	EC_KEY* secpKey = nullptr;
	secpKey = EC_KEY_new_by_curve_name(NID_secp256k1);
	EC_KEY_generate_key(secpKey);

	int a =ECDSA_size(secpKey);

	BIGNUM const* prv = EC_KEY_get0_private_key(secpKey);
	EC_POINT const* pub = EC_KEY_get0_public_key(secpKey);

	this->ecKey = secpKey;
	this->nodeID = nodeID;
}

Transaction UserNode::createProductTX(char* modelNo, int price, char* others)
{

	Product* product = new Product(time(NULL), this->identifier, modelNo);
	Transaction* tx = new Transaction(NULL, this->ecKey, price, others, product);

	//trID�� ���ϴ� ����
	string hashRes = hashTX(tx);
	char* c = const_cast<char*>(hashRes.c_str());
	tx->setTrID(c);

	//trID�� ������ ��ü tx�� hash�ؼ� ��üTX�� hash���� ���Ѵ�.
	hashRes = hashTX(tx);
	c = const_cast<char*>(hashRes.c_str());
	tx->setHashTx(c);

	//privateKey�� �̿��ؼ� �����Ѵ�.
	ECDSA_SIG* sig = signTX(hashRes);	
	tx->setSig(sig);

	return *tx;
}

Transaction UserNode::sellProductTX(Product* product, EC_KEY* dest, char* others, int price)
{
	
	Transaction* tx = new Transaction(this->ecKey, dest, price, others, product);

	//trID�� ���ϴ� ����
	string hashRes = hashTX(tx);
	char* c = const_cast<char*>(hashRes.c_str());
	tx->setTrID(c);

	//trID�� ������ ��ü tx�� hash�ؼ� ��üTX�� hash���� ���Ѵ�.
	hashRes = hashTX(tx);
	c = const_cast<char*>(hashRes.c_str());
	tx->setHashTx(c);

	//privateKey�� �̿��ؼ� �����Ѵ�.
	ECDSA_SIG* sig = signTX(hashRes);
	tx->setSig(sig);

	return *tx;
}

ECDSA_SIG* UserNode::signTX(string hashRes)
{
	const char* c = hashRes.c_str();
	ECDSA_SIG* sig;
	EC_KEY* key = this->ecKey;
	sig = ECDSA_do_sign((unsigned char*)c, 20, this->ecKey);
	return sig;
}

int UserNode::getIdentifier() { return this->identifier; }

EC_KEY* UserNode::getEcKey() { return this->ecKey; }
