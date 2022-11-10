#pragma once
#include"utilHeader.h"

/*----------
	MERKLE TREE
------------*/

class MerkleTree {};

/*----------
	PRODUCT
------------*/

class Product
{
private:
	time_t manufacturedDate;
	string modelNo;
	int identifier;

public:
	Product() {};

	Product(time_t date, string modelNo, int identifier)
	{
		this->manufacturedDate = date;
		this->modelNo = modelNo;
		this->identifier = identifier;
	};

};

/*----------
	TRANSACTION
------------*/

class Transaction {
private:
	unsigned char* trID;
	char *input; // 송신자의 pub key
	char *ouput; // 수신자의 pub key
	int price;
	time_t tradingDate;
	string others;
	Product product;
	ECDSA_SIG* sig;

public:
	Transaction(char* input, char* output, int price, string others, Product* product)
	{
		this->input = input;
		this->ouput = output;
		this->price = price;
		this->others = others;

		this->tradingDate = time(nullptr);
		this->product = *product;
	}

	void setTrID(unsigned char* trID) { this->trID = trID; }
	void setSig(ECDSA_SIG* sig) { this->sig = sig; }
		
};