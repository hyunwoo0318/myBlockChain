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
	EC_KEY *input; // 송신자의 pub key
	EC_KEY *output; // 수신자의 pub key
	ECDSA_SIG* sig;
	int price;
	time_t tradingDate;	
	
	char trID[256] = "";
	char hashTx[256];
	char others[50];

	Product product;
public:
	Transaction() {}
	Transaction(EC_KEY* input, EC_KEY* output, int price, char* others, Product* product)
	{
		this->input = input;
		this->output = output;
		this->price = price;
		strcpy(this->others, others);

		this->tradingDate = time(nullptr);
		this->product = *product;
	}

	void setTrID(char* trID) { strcpy(this->trID, trID); }
	char* getTrID() { return this->trID; }
	void setSig(ECDSA_SIG* sig) { this->sig = sig; }
	void setHashTx(char* hashTx) { strcpy(this->hashTx, hashTx); }
	char* getHashTx() { return this->hashTx; }
	EC_KEY* getInput() { return this->input; }
	EC_KEY* getOutput() { return this->output; }
	Product getProduct() { return this->product; }
	ECDSA_SIG* getSig() { return this->sig; }
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
	long blockNo;
	HashPointer prev;	
	string mrklRootHash;
public:
	uint32_t nonce;
	Header() {}
	Header(long blockNo, HashPointer prev, string mrklRootHash)
		:blockNo(blockNo), prev(prev), mrklRootHash(mrklRootHash) {}
	Header(long blockNo, HashPointer prev, uint32_t nonce,
		string mrklRootHash);	
	long getBlockNo() { return this->blockNo; }
	void setHashPointer(HashPointer hashPointer) { this->prev = hashPointer; }
	void setMrklRootHash(string mrklRootHash) { this->mrklRootHash = mrklRootHash; }
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
	
	Block() {}
	
	Block(Header header, MerkleTree merkleTree)
	{
		this->header = header;
		this->merkleTree = merkleTree;
		string hashRes = hashTX(&header);
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
	vector<Block> blockChain;
public:

	BlockChain() {}
	void insertBlock(Block block);
	Block getLastBlock();
	//TODO : 해당 identifier를 가진 product를 소유한 주인을 찾음
	EC_KEY* findProductOwner(int identifier);
	Product findProduct(int identifier);
};


