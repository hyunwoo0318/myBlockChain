#pragma once
#include<random>
#include"data.h"

class UserNode
{
private:
	EC_KEY* ecKey;
	int identifier;
	int nodeID;
public:
	UserNode(int nodeID);	

	Transaction createProductTX(string modelNo, int price, string others);
	

	Transaction sellProductTX(Product* product, EC_KEY* dest, string others, int price);


	ECDSA_SIG* signTX(string hashRes);

	int getIdentifier();
	EC_KEY* getEcKey();
};

