#pragma once
#include"utilHeader.h"
#include"data.h"
/*
UserNode

최초의 TX에서는 적절한 개수의 물품을 생성 - 3개의 물품 생성예정
이후의 TX에서는 자신의 물품 (Identifier로 구별)을 판매하는 TX를 생성

TX를 생성하면 다른 FullNode들에게 전달한다.
*/

class UserNode
{
private:
	char *privateKey;
	char *publicKey;
	int identifier;
	int nodeID;
public:
	UserNode() {};

	UserNode(int nodeID);

	pair<char*, char*> GenerateKeyPair();

	Transaction createProductTX(string modelNo, int price, string others);

	Transaction sellProductTX(Product* product, char* dest, string others, int price);

	unsigned char* hashTX(Transaction* tx);

	ECDSA_SIG* signTX(Transaction* tx, unsigned char* hashRes);
};
//TODO : tx를 일정한 간격으로 생성해야함.

//TODO : full node로 전송해야함.

//TODO : full node로 부터는 데이터를 받지않음.