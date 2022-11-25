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

	Transaction createProductTX(char* modelNo, int price, char* others);
	

	Transaction sellProductTX(Product* product, EC_KEY* dest, char* others, int price);


	ECDSA_SIG* signTX(string hashRes);

	int getIdentifier();
	EC_KEY* getEcKey();
};

