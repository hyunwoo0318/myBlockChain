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
	string trID="";
	EC_KEY *input; // 송신자의 pub key
	EC_KEY *output; // 수신자의 pub key
	int price;
	time_t tradingDate;
	string others;
	Product product;
	//for verify tx
	string hashTx;
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

	void setTrID(string trID) { this->trID = trID; }
	string getTrID() { return this->trID; }
	void setSig(ECDSA_SIG* sig) { this->sig = sig; }
	void setHashTx(string hashTx) { this->hashTx = hashTx; }
	string getHashTx() { return this->hashTx; }
	EC_KEY* getInput() { return this->input; }
	EC_KEY* getOutput() { return this->output; }
	Product getProduct() { return this->product; }
	ECDSA_SIG* getSig() { return this->sig; }		
};

struct Node
{
	pair<string, string> value;
};

/*----------
	MERKLE TREE
------------*/

class MerkleTree
{
private:
	vector<Node> node[100];
	vector<Transaction> leafNode[100];
public:
	MerkleTree() {}
	//TODO : 함수 구현
	// 시작할때 tx의 순서를 무작위로 섞음 -> nonce값 찾기 실패했을때를 대비
	void makeMerkleTree(vector<Transaction> tx);
	//해당 block에 존재하는 모든 TX찾기.
	vector<Transaction> findAllTX();

};

/*----------
	HASH POINTER
------------*/
class HashPointer
{
private:
	Block* pointer = nullptr;
	string prevHash;
public:
	HashPointer() {}
	HashPointer(Block* pointer, string prevHash)
	{
		this->pointer = pointer;
		this->prevHash = prevHash;
	}
};



/*----------
	HEADER
------------*/
class Header
{
private:
	long blockNo;
	HashPointer prev;	
	string mrkl_root_hash;
public:
	uint32_t nonce;
	Header() {}
	Header(long blockNo, HashPointer prev, uint32_t nonce,
		string mrkl_root_hash);	
	void setHashPointer(HashPointer hashPointer) { this->prev = hashPointer; }
};

/*----------
	BLOCK
------------*/
class Block
{
private:
	Header header;
	MerkleTree merkleTree;
	string hashRes;
public:
	Block(Header header, MerkleTree merkleTree)
	{
		this->header = header;
		this->merkleTree = merkleTree;
		string hashRes = hashTX(header) + hashTX(merkleTree);
		this->hashRes = hashRes;
	}
	void changeHeaderHash(string headerHashRes)
	{
		string hashRes = headerHashRes + hashTX(this->merkleTree);
		this->hashRes = hashRes;
	}
	Header getHeader() { return this->header; }
	void setHashRes(string hashRes) { this->hashRes = hashRes; }
	string getHashRes() { return hashRes; }
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


