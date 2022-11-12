#include"UserNode.h"
#include<random>


//identifier, publicKey, privateKey를 배정
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

////ECDSA를 이용해서 pubKey, privateKey를 생성
//pair<char*, char*> UserNode::GenerateKeyPair()
//{
//	EC_KEY* secpKey = nullptr;
//	const BIGNUM* privateKey = nullptr;
//	const EC_POINT* publicKey = nullptr;
//
//	char* privateKeyString;
//	char* publicKeyString;
//
//	//secp256k1으로 부터 key를 만듬
//	secpKey = EC_KEY_new_by_curve_name(NID_secp256k1);
//
//	//위에서 만든 키를 이용해서 privateKey를 생성
//	EC_KEY_set_private_key(secpKey, privateKey);
//	auto val = EC_KEY_get0_private_key(secpKey);
//	privateKeyString = BN_bn2hex(privateKey); // 매우 큰 수를 string으로 변환
//
//	//동일한 키를 이용해서 publicKey를 생성
//	EC_GROUP* group;
//	group = EC_GROUP_new_by_curve_name(NID_secp256k1);
//	EC_KEY_set_public_key(secpKey, publicKey);
//	publicKey = EC_KEY_get0_public_key(secpKey);
//	publicKeyString = EC_POINT_point2hex(group, publicKey, POINT_CONVERSION_COMPRESSED, NULL);
//	EC_GROUP_free(group);
//
//	return { publicKeyString, privateKeyString };
//
//}

//최초의 물건을 생성하는 TX생성
Transaction UserNode::createProductTX(string modelNo, int price,string others)
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

//물건을 판매하는 TX 생성
Transaction UserNode::sellProductTX(Product* product, EC_KEY* dest, string others, int price)
{	
	string hashRes;
	Transaction* tx = new Transaction(this->ecKey,dest, price, others, product);	

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


//전체 tx를 hash한 값을 이용해 서명한다.
ECDSA_SIG* UserNode::signTX(string hashRes)
{
	const char* c = hashRes.c_str();
	ECDSA_SIG* sig;	
	EC_KEY* key = this->ecKey;
	sig = ECDSA_do_sign((unsigned char*) c, 20, this->ecKey);
	return sig;
}






