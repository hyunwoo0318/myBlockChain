#include"utilHeader.h"
#include"data.h"
#include"UserNode.h"

int main()
{
	UserNode* userNode1 = new UserNode(1);
	UserNode* userNode2 = new UserNode(2);
	Transaction tx1 = userNode1->createProductTX("car", 13000, "usernode1 others");
	Transaction tx2 = userNode2->createProductTX("bus", 7800, "usernode2 others");
	EC_KEY_generate_key(userNode1->getEcKey());
	EC_KEY_generate_key(userNode2->getEcKey());

	int trueCase = ECDSA_do_verify(tx1.getHashTx(), 20, tx1.getSig(), userNode1->getEcKey());
	int falseCase = ECDSA_do_verify(tx2.getHashTx(), 20, tx2.getSig(), userNode2->getEcKey());
	cout << trueCase << falseCase;

	char st [] = "acasdas";
	int ret,ret2;
	ECDSA_SIG* sig;
	EC_KEY* key;
	EC_KEY* key2;
	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	key2 = EC_KEY_new_by_curve_name(NID_secp192k1);
	EC_KEY_generate_key(key);
	EC_KEY_generate_key(key2);

	sig = ECDSA_do_sign((unsigned char*)&st, 20, key2);
	ret = ECDSA_do_verify((unsigned char*)&st, 20, sig, key);
	ret2 = ECDSA_do_verify((unsigned char*)&st, 20, sig, key2);
	cout << ret << ret2;


}
