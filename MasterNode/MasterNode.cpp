#include"pch.h"
#include"data.h"
#include"UserNode.h"
#pragma warning(disable : 4996)

TCHAR buf[BUFSIZE * 5000];

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


	//TEST mrklTree
	char msg1[] = "msg1";
	char msg2[] = "msg2";
	char msg3[] = "msg3";
	char md[] = "modelNo";

	vector<Transaction> vv;

	Product * p = new Product(time(nullptr), 123123, md);

	Transaction* t1 = u->sellProductTX(p, destPub, msg1, 11111);
	Transaction* t2 = u->sellProductTX(p, destPub, msg2, 22222);
	Transaction* t3 = u->sellProductTX(p, destPub, msg3, 33333);

	vv.push_back(*t1);
	vv.push_back(*t2);
	vv.push_back(*t3);

	MerkleTree* mk = new MerkleTree();
	mk->makeMerkleTree(vv);
	/*mk->serialize(buf);

	MerkleTree* mm = new MerkleTree();
	mm->deserialize(buf);

	string s = (mm->node)[0].value.first;
	string ss = (mk->node)[0].value.first;

	if ((mm->node)[0].value.first == (mk->node)[0].value.first)
		cout << 1 << endl;
	else
		cout << 2 << endl;*/

	//TEST block

	/*char prevHash[32] = "33333333333333";
	char mrkl[64] = "999999999999999999999999";
	HashPointer* hp = new HashPointer(NULL, prevHash);
	Header* h = new Header(3, *hp, mrkl);
	h->nonce = 111;

	Block* b = new Block(*h, mk);
	b->serialize(buf);

	Block* c = new Block();
	c->deSerialize(buf);*/


}
