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

	char trID[32] = "";
	char hashTx[32]="";
	char others[20]="";

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
	int serialize(TCHAR* buf);
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
public:
	int nodeNum;
	int txNum;
	vector<Node> node;
	vector<Transaction> leafNode;

	MerkleTree() 
	{
		node.resize(30);
		leafNode.resize(30);
	}	
	// 시작할때 tx의 순서를 무작위로 섞음 -> nonce값 찾기 실패했을때를 대비
	void makeMerkleTree(vector<Transaction> &tx);
	//해당 block에 존재하는 모든 TX찾기.
	vector<Transaction> findAllTX();
	string getRootHash();
	int serialize(TCHAR* buf);
	void deserialize(TCHAR* buf);
};


class Block;
/*----------
	HASH POINTER
------------*/
class HashPointer
{
private:
	Block* pointer = nullptr;
	char prevHash[32];
public:
	HashPointer() {}
	HashPointer(Block* pointer, char* prevHash)
	{
		this->pointer = pointer;
		strcpy(this->prevHash, prevHash);
	}
	void setPrevHash(char* prevHash)
	{
		strcpy(this->prevHash, prevHash);
	}
	char* getPrevHash() { return this->prevHash; }

};



/*----------
	HEADER
------------*/
class Header 
{
private:
	int blockNo;
	HashPointer prev;	
	char mrklRootHash[64];
public:
	unsigned int nonce;
	Header() {}
	Header(int blockNo, HashPointer prev, char* mrklRootHash)
		:blockNo(blockNo), prev(prev) 
	{
		strcpy(this->mrklRootHash, mrklRootHash);
	}
	Header(int blockNo, HashPointer prev, unsigned int nonce,
		char* mrklRootHash);	
	int getBlockNo() { return this->blockNo; }
	char* getMrklRootHash() { return this->mrklRootHash; }
	HashPointer* getHashPointer() { return &(this->prev); }
	void setBlockNo(int blockNo) { this->blockNo = blockNo; }
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
	char hashRes[32];
	MerkleTree* merkleTree;
public:	
	Block()
	{
		this->merkleTree = new MerkleTree();
	}
	
	Block(Header header, MerkleTree* merkleTree)
	{
		this->header = header;
		this->merkleTree = merkleTree;
		string hashRes = hashTX(&header);
		strcpy(this->hashRes, hashRes.c_str());
	}
	Header getHeader() { return this->header; }
	void setHashRes(char* hashRes) { strcpy(this->hashRes, hashRes); }
	char* getHashRes() { return hashRes; }
	void serialize(TCHAR* buf);
	void deSerialize(TCHAR* buf);
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

