#include"UserNode.h"
#include<random>


//identifier, publicKey, privateKey를 배정
UserNode::UserNode(int nodeID)
{
	random_device rd;
	mt19937 gen(rd());
	this->identifier = gen();

	UserNode* userNode = new UserNode();
	pair<char*, char*> keyPair = userNode->GenerateKeyPair();
	delete userNode;

	this->privateKey = keyPair.second;
	this->publicKey = keyPair.first;
	this->nodeID = nodeID;
}

//ECDSA를 이용해서 pubKey, privateKey를 생성
pair<char*, char*> UserNode::GenerateKeyPair()
{
	EC_KEY* secpKey;
	BIGNUM* privateKey;
	EC_POINT* publicKey;

	char* privateKeyString;
	char* publicKeyString;

	//secp256k1으로 부터 key를 만듬
	secpKey = EC_KEY_new_by_curve_name(NID_secp256k1);

	//위에서 만든 키를 이용해서 privateKey를 생성
	privateKey = (BIGNUM*)EC_KEY_get0_private_key(secpKey);
	privateKeyString = BN_bn2hex(privateKey); // 매우 큰 수를 string으로 변환

	//동일한 키를 이용해서 publicKey를 생성
	EC_GROUP* group;
	group = EC_GROUP_new_by_curve_name(NID_secp256k1);
	publicKey = (EC_POINT*)EC_KEY_get0_public_key(secpKey);
	publicKeyString = EC_POINT_point2hex(group, publicKey, POINT_CONVERSION_COMPRESSED, NULL);
	EC_GROUP_free(group);

	return { publicKeyString, privateKeyString };

}

//최초의 물건을 생성하는 TX생성
Transaction UserNode::createProductTX(string modelNo, int price,string others)
{
	unsigned char* hashRes;
	Product* product = new Product(time(NULL), modelNo, this->identifier);
	Transaction* tx = new Transaction(NULL, this->publicKey, price, others, product);

	//trID를 구하는 과정
	UserNode* userNode = new UserNode();
	hashRes = userNode->hashTX(tx);
	tx->setTrID(hashRes);

	//trID를 포함한 전체 tx를 hash해서 전체TX의 hash값을 구한다.
	hashRes = userNode->hashTX(tx);
	delete userNode;

	//privateKey를 이용해서 서명한다.
	ECDSA_SIG* sig = signTX(tx, hashRes);
	tx->setSig(sig);

	return *tx;
}

//물건을 판매하는 TX 생성
Transaction UserNode::sellProductTX(Product* product, char* dest, string others, int price)
{	
	unsigned char* hashRes;
	Transaction* tx = new Transaction(this->publicKey,dest, price, others, product);

	//trID를 제외한 전체 부분을 hash해서 trID를 구함
	UserNode* userNode = new UserNode();
	hashRes = userNode->hashTX(tx);
	tx->setTrID(hashRes);

	//trID를 포함한 전체 tx를 hash해서 전체TX의 hash값을 구한다.
	hashRes = userNode->hashTX(tx);
	delete userNode;

	//privateKey를 이용해서 서명한다.
	ECDSA_SIG* sig = signTX(tx, hashRes);
	tx->setSig(sig);

	return *tx;
}

//tx를 hash함
unsigned char* UserNode::hashTX(Transaction* tx)
{
	unsigned char* hashRes;
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, tx, sizeof(tx));
	SHA256_Final(hashRes, &ctx);
	return hashRes;
}


//전체 tx를 hash한 값을 구한다.
ECDSA_SIG* UserNode::signTX(Transaction* tx, unsigned char* hashRes)
{
	ECDSA_SIG* sig;
	sig = ECDSA_do_sign(hashRes, 32, (EC_KEY*)this->privateKey);
	return sig;
}






