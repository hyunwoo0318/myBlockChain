#include "pch.h"
#include "UserNode.h"


UserNode::UserNode(int nodeID)
{
	EC_GROUP* group;
	random_device rd;
	mt19937 gen(rd());
	this->identifier = gen();

	EC_KEY* secpKey = nullptr;
	secpKey = EC_KEY_new_by_curve_name(NID_secp256k1);
	EC_KEY_generate_key(secpKey);

	BIGNUM const* prv = EC_KEY_get0_private_key(secpKey);
	EC_POINT const* pub = EC_KEY_get0_public_key(secpKey);

	this->ecKey = secpKey;
	this->nodeID = nodeID;
}

Transaction UserNode::createProductTX(string modelNo, int price, string others)
{

	Product* product = new Product(time(NULL), modelNo, this->identifier);
	Transaction* tx = new Transaction(NULL, this->ecKey, price, others, product);

	//trID를 구하는 과정
	string hashRes = hashTX(tx);
	tx->setTrID(hashRes);

	//trID를 포함한 전체 tx를 hash해서 전체TX의 hash값을 구한다.
	hashRes = hashTX(tx);
	tx->setHashTx(hashRes);

	//privateKey를 이용해서 서명한다.
	ECDSA_SIG* sig = signTX(hashRes);
	tx->setSig(sig);

	return *tx;
}

Transaction UserNode::sellProductTX(Product* product, EC_KEY* dest, string others, int price)
{
	string hashRes;
	Transaction* tx = new Transaction(this->ecKey, dest, price, others, product);

	//trID를 구하는 과정
	hashRes = hashTX(tx);
	tx->setTrID(hashRes);

	//trID를 포함한 전체 tx를 hash해서 전체TX의 hash값을 구한다.
	hashRes = hashTX(tx);
	tx->setHashTx(hashRes);

	//privateKey를 이용해서 서명한다.
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
