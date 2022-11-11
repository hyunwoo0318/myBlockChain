#pragma once
#include"utilHeader.h"


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

	string getModelNo() { return this->modelNo; }
	int getIdentifier() { return this->identifier; }
	time_t getDate() { return this->manufacturedDate; }

	bool operator==(Product& product)
	{
		if (product.identifier == this->identifier
			&& product.manufacturedDate == this->manufacturedDate
			&& product.modelNo == this->modelNo)
			return true;
		else return false;
	}


};

/*----------
	TRANSACTION
------------*/

class Transaction {
private:
	unsigned char* trID;
	EC_KEY *input; // 송신자의 pub key
	EC_KEY *output; // 수신자의 pub key
	int price;
	time_t tradingDate;
	string others;
	Product product;
	//TODO : 외부에서 변경할수있는 가능성이있음 -> 없애기
	const unsigned char * hashTx; // for verify signature
	ECDSA_SIG* sig;

public:
	Transaction(EC_KEY* input, EC_KEY* output, int price, string others, Product* product)
	{
		this->input = input;
		this->output = output;
		this->price = price;
		this->others = others;

		this->tradingDate = time(nullptr);
		this->product = *product;
	}

	void setTrID(unsigned char* trID) { this->trID = trID; }
	void setSig(ECDSA_SIG* sig) { this->sig = sig; }
	void setHashTx(const unsigned char* hashTx) { this->hashTx = hashTx; }
	const unsigned char* getHashTx() { return this->hashTx; }
	EC_KEY* getInput() { return this->input; }
	EC_KEY* getOutput() { return this->output; }
	Product getProduct() { return this->product; }
	ECDSA_SIG* getSig() { return this->sig; }
		
};


/*----------
	MERKLE TREE
------------*/

class MerkleTree
{
private:
	unsigned char* node;
	Transaction tx;
public:
	//TODO : 함수 구현
	// 시작할때 tx의 순서를 무작위로 섞음 -> nonce값 찾기 실패했을때를 대비
	MerkleTree insertTX(Transaction tx);
	//해당 block에 존재하는 모든 TX찾기.
	vector<Transaction> findAllTX();

};



/*----------
	HEADER
------------*/
class Header
{
private:
	long blockNo;
	unsigned char* prev;	
	unsigned char* mrkl_root_hash;
public:
	uint32_t nonce;
	Header(long blockNo, unsigned char* prev, uint32_t nonce,
		unsigned char* mrkl_root_hash);
	
};

/*----------
	BLOCK
------------*/
class Block
{
private:
	Header header;
	MerkleTree merkleTree;
	unsigned char* hashRes;
public:
	unsigned char* getHashRes() { return hashRes; }
};

/*----------
	BLOCKCHAIN
------------*/
class BlockChain
{
private :
	Block block;
public:
	void insertBlock(Block prevBlock, Block curBlock);
	//TODO : 해당 identifier를 가진 product를 소유한 주인을 찾음
	EC_KEY* findProductOwner(int identifier);
	Product findProduct(int identifier);
};

