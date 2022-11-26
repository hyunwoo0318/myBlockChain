#include "pch.h"
#include "UserNode.h"


UserNode::UserNode(int nodeID)
{
	random_device rd;
	mt19937 gen(rd());
	this->identifier = gen();
		
	this->nodeID = nodeID;
	setKeyList();
	this->ecKey = (this->keyList)[nodeID];
}

Transaction UserNode::createProductTX(char* modelNo, int price, char* others)
{

	Product* product = new Product(time(NULL), this->identifier, modelNo);
	Transaction* tx = new Transaction(NULL, getPubKey(), price, others, product);

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
	const BIGNUM* R = ECDSA_SIG_get0_r(sig);
	const BIGNUM* S = ECDSA_SIG_get0_s(sig);
	char* sigR = BN_bn2hex(R);
	char* sigS = BN_bn2hex(S);

	tx->setSigR(sigR);
	tx->setSigS(sigS);

	return *tx;
}

Transaction* UserNode::sellProductTX(Product* product, char* dest, char* others, int price)
{
	
	Transaction* tx = new Transaction(getPubKey(), dest, price, others, product);

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
	const BIGNUM* R = ECDSA_SIG_get0_r(sig);
	const BIGNUM* S = ECDSA_SIG_get0_s(sig);
	char* sigR = BN_bn2hex(R);
	char* sigS = BN_bn2hex(S);

	tx->setSigR(sigR);
	tx->setSigS(sigS);

	return tx;
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

void UserNode::setEcKey(const char* priv)
{
	EC_KEY* ecKey = makeEcKey(priv);
	this->ecKey = ecKey;
}

char* UserNode::getPubKey()
{
	const EC_POINT* pubKey = EC_KEY_get0_public_key(this->ecKey);
	const EC_GROUP* group = EC_KEY_get0_group(this->ecKey);
	char *buf;
	buf = EC_POINT_point2hex(group, pubKey, POINT_CONVERSION_UNCOMPRESSED, NULL);
	return buf;
}

EC_KEY* UserNode::findKey(char* pubKey)
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

void UserNode::setKeyList()
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
