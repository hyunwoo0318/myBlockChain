#pragma once
#include<random>
#include"data.h"

class UserNode
{
private:
	EC_KEY* ecKey;
	int identifier;
	int nodeID;
	vector<EC_KEY*> keyList;
public:
	UserNode(int nodeID);	

	Transaction* createProductTX(char* modelNo, int price, char* others);
	

	Transaction* sellProductTX(Product* product, char* dest, char* others, int price);


	ECDSA_SIG* signTX(string hashRes);

	int getIdentifier();
	EC_KEY* getEcKey();
	void setEcKey(const char* priv);
	char* getPubKey();
	vector<EC_KEY*> getKeyList() { return this->keyList; }

	EC_KEY* findKey(char* pubKey);
	void setKeyList();
};

