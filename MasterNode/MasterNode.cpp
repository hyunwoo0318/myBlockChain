#include"pch.h"
#include"data.h"
#include"UserNode.h"
#pragma warning(disable : 4996)

int main()
{	
	UserNode* u = new UserNode(1);	
	
	vector<EC_KEY*> keyList = u->getKeyList();
	EC_KEY* dest = keyList[2];
	const EC_POINT* d = EC_KEY_get0_public_key(dest);
	const EC_GROUP* g= EC_KEY_get0_group(dest);
	char* destPub = EC_POINT_point2hex(g, d, POINT_CONVERSION_UNCOMPRESSED, NULL);
	//EC_KEY* k1 = makeEcKey(PRIV0);
	//EC_KEY* k2 = makeEcKey(PRIV0);
	//EC_KEY* k3 = EC_KEY_new();
	//const EC_POINT* pub = EC_KEY_get0_public_key(k1);
	//int rrrrrr = EC_KEY_set_public_key(k3, pub);
	//cout << rrrrrr << endl;
	//char msg[] = "hello";
	//string res = hashTX(msg);
	//ECDSA_SIG* sig = ECDSA_do_sign((unsigned char*)res.c_str(), 32, k1);
	//const BIGNUM* r = BN_new();
	//const BIGNUM* s = BN_new();
	//r = ECDSA_SIG_get0_r(sig);
	//s = ECDSA_SIG_get0_s(sig);
	//char *a = BN_bn2hex(r);
	//char *b = BN_bn2hex(r);
	//BIGNUM* rr = const_cast<BIGNUM*>(r);
	//BIGNUM* ss= const_cast<BIGNUM*>(s);
	//ECDSA_SIG* sigF = ECDSA_SIG_new();
	//int xxx= ECDSA_SIG_set0(sigF,rr,ss);
	//cout << xxx << endl;
	//if (sigF == sig)
	//	cout << 123 << endl;
	//else
	//	cout << 1234123 << endl;
	//int ret1 = ECDSA_do_verify((unsigned char*)res.c_str(), 32, sig, k1);
	//int ret2 = ECDSA_do_verify((unsigned char*)res.c_str(), 32, sig, k2);
	//int ret3 = ECDSA_do_verify((unsigned char*)res.c_str(), 32, sigF, k1);
	//int ret4 = ECDSA_do_verify((unsigned char*)res.c_str(), 32, sigF, k2);
	////int ret3 = ECDSA_do_verify((unsigned char*)res.c_str(), 32, sig, k3);
	//cout << ret1 << " " << ret2 << endl << ret3 << endl << ret4 << endl;
	//if (k1 == k2)
	//	cout << 2;
	//else
	//	cout << 3;
	//

	char msg[] = "modelno";
	char others[] = "others";
	Product * p = new Product(time(nullptr), 123123, msg);

	Transaction* t = u->sellProductTX(p, destPub, others, 13000);
	TCHAR buf[BUFSIZE * 3];
	t->serialize(buf);
	Transaction* tx = new Transaction();
	tx->deserialize(buf);

	if (*(t->getHashTx()) == *(tx->getHashTx()))
		cout << 1 << endl;
	else
		cout <<  2 << endl;

	if (*(t->getInput()) == *(tx->getInput()))
		cout << 1 << endl;
	else
		cout << 2 << endl;

	if (*(t->getOutput()) == *(tx->getOutput()))
		cout << 1 << endl;
	else
		cout << 2 << endl;

	if (*(t->getSigR()) == *(tx->getSigR()))
		cout << 1 << endl;
	else
		cout << 2 << endl;

	if (*(t->getSigS()) == *(tx->getSigS()))
		cout << 1 << endl;
	else
		cout << 2 << endl;

	if (*(t->getTrID()) == *(tx->getTrID()))
		cout << 1 << endl;
	else
		cout << 2 << endl;





}