#pragma once

/*----------
	PRODUCT
------------*/

class Product
{
private:
	time_t manufacturedDate;	
	int identifier;
	char modelNo[50];

public:
	Product() {};

	Product(time_t date,int identifier , char* modelNo )
	{
		this->manufacturedDate = date;
		strcpy(this->modelNo, modelNo);
		this->identifier = identifier;
	};

	char* getModelNo() { return this->modelNo; }
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

	void serialize(TCHAR* retBuf);

	void deSerialize(TCHAR* retBuf);

};

/*----------
	TRANSACTION
------------*/

class Transaction {
private:	
	char input[130]; // 송신자의 pub key
	char output[130]; // 수신자의 pub key
	char sigR[64];
	char sigS[64];

	char trID[256] = "";
	char hashTx[256]="";
	char others[50]="";

	Product product;

	time_t tradingDate;
	int price;
		
public:
	Transaction(){}
	Transaction(char* input, char* output, int price, char* others, Product* product)
	{
		strcpy(this->input, input);
		strcpy(this->output, output);
		this->price = price;
		strcpy(this->others, others);

		this->tradingDate = time(nullptr);
		this->product = *product;
	}

	void setTrID(char* trID) { strcpy(this->trID, trID); }
	char* getTrID() { return this->trID; }
	void setSigR(char* sigR) { strcpy(this->sigR, sigR); }
	void setSigS(char* sigS) { strcpy(this->sigS, sigS); }
	void setHashTx(char* hashTx) { strcpy(this->hashTx, hashTx); }
	char* getHashTx() { return this->hashTx; }
	char* getInput() { return this->input; }
	char* getOutput() { return this->output; }
	Product getProduct() { return this->product; }
	char* getSigR() { return this->sigR; }
	char* getSigS() { return this->sigS; }
	void serialize(TCHAR* buf);
	void deserialize(TCHAR* buf);
};

struct Node
{
	//양 child의 hash값을 가짐
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
	// 시작할때 tx의 순서를 무작위로 섞음 -> nonce값 찾기 실패했을때를 대비
	void makeMerkleTree(const vector<Transaction> &tx);
	//해당 block에 존재하는 모든 TX찾기.
	vector<Transaction> findAllTX();
	string getRootHash();

};


class Block;
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
	int blockNo;
	HashPointer prev;	
	char mrklRootHash[512];
public:
	unsigned int nonce;
	Header() {}
	Header(int blockNo, HashPointer prev, char* mrklRootHash)
		:blockNo(blockNo), prev(prev) 
	{
		strcpy(this->mrklRootHash, mrklRootHash);
	}
	Header(long blockNo, HashPointer prev, unsigned int nonce,
		string mrklRootHash);	
	int getBlockNo() { return this->blockNo; }
	void setHashPointer(HashPointer hashPointer) { this->prev = hashPointer; }
	void setMrklRootHash(char* mrklRootHash) { strcpy(this->mrklRootHash, mrklRootHash); }
};

/*----------
	BLOCK
------------*/
class Block
{
private:
	Header header;
	MerkleTree merkleTree;
	char hashRes[256];
public:
	
	Block() {}
	
	Block(Header header, MerkleTree merkleTree)
	{
		this->header = header;
		this->merkleTree = merkleTree;
		string hashRes = hashTX(&header);
		strcpy(this->hashRes, hashRes.c_str());
	}
	Header getHeader() { return this->header; }
	void setHashRes(char* hashRes) { strcpy(this->hashRes, hashRes); }
	char* getHashRes() { return hashRes; }
};

/*----------
	BLOCKCHAIN
------------*/
class BlockChain 
{
private :
	vector<Block> blockChain;
public:

	BlockChain() {}
	void insertBlock(Block block);
	Block getLastBlock();
	//TODO : 해당 identifier를 가진 product를 소유한 주인을 찾음
	char* findProductOwner(int identifier);
	Product findProduct(int identifier);
};

EC_KEY* makeEcKey(const char* priv);

