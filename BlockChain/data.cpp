#include "pch.h"
#include"data.h"

TCHAR tempBuf[BUFSIZE * 500];

/*----------
	HEADER
------------*/

Header::Header(int blockNo, HashPointer prev, unsigned int nonce,
	char* mrklRootHash)
{
	this->blockNo = blockNo;
	this->prev = prev;
	this->nonce = nonce;
	strcpy(this->mrklRootHash, mrklRootHash);
}

/*----------
	MERKLE TREE
------------*/

void MerkleTree::makeMerkleTree(vector<Transaction> &tx)
{
	//tx의 개수
	int txNum = tx.size();
	for (int i = 0; i < txNum; i++)
	{
		(this->leafNode)[i] = tx[i];
	}
	//노드의 개수 -> 포화이진트리의 성질
	int nodeNum = txNum - 1;	

	this->nodeNum = nodeNum;
	this->txNum = txNum;

	//부모의 idx가 x라면 자식의 idx는 2x+1, 2x+2이다.
	//자식이 둘다 tx, 하나만 tx, 모두 node인 경우를 분리해 순서대로 hash값을 저장한다.

	//1)tx의 개수가 짝수개인경우 -> 자식중 하나만 tx인 경우는 존재하지않음.
	if (txNum % 2 == 0)
	{
		int idx = 0;
		int bound = (txNum - 2) / 2;

		//자식이 둘다 tx인경우
		for (int i = bound; i < nodeNum; i++)
		{
			(this->node)[i].value.first = (this->leafNode)[idx++].getHashTx();
			(this->node)[i].value.second = (this->leafNode)[idx++].getHashTx();
		}

		//자식이 둘다 노드인경우
		for (int i = 0; i < bound; i++)
		{			
			Node leftChild = (this->node)[2 * i + 1];
			Node rightChild = (this->node)[2 * i + 2];

			//각 노드의 값을 더해서 hash를함
			string leftValue = leftChild.value.first + leftChild.value.second;
			string rightValue = rightChild.value.first + rightChild.value.second;

			string hashLeftValue = hashTX(&leftValue);
			string hashRightValue = hashTX(&rightValue);

			(this->node)[i].value.first = hashLeftValue;
			(this->node)[i].value.second = hashRightValue;
		}
	}
	//2)tx의 개수가 홀수개인경우 -> 자식중 하나만 tx인 1개 존재함 => bound
	else
	{
		int idx = 0;
		int bound = (txNum - 3) / 2;

		//자식이 둘다 tx인경우
		for (int i = bound+1; i < nodeNum; i++)
		{
			(this->node)[i].value.first = (this->leafNode)[idx++].getHashTx();
			(this->node)[i].value.second = (this->leafNode)[idx++].getHashTx();
		}

		//자식이 둘다 노드인경우
		for (int i = 0; i < bound; i++)
		{
			Node leftChild = (this->node)[2 * i + 1];
			Node rightChild = (this->node)[2 * i + 2];

			//각 노드의 값을 더해서 hash를함
			string leftValue = leftChild.value.first + leftChild.value.second;
			string rightValue = rightChild.value.first + rightChild.value.second;

			string hashLeftValue = hashTX(&leftValue);
			string hashRightValue = hashTX(&rightValue);

			(this->node)[i].value.first = hashLeftValue;
			(this->node)[i].value.second = hashRightValue;
		}

		//자식이 하나는 tx, 하나는 노드인경우
		{
			Node boundChild = (this->node)[2 * bound + 1];
			string childValue = boundChild.value.first + boundChild.value.second;
			string hashValue = hashTX(&childValue);
			(this->node)[bound].value.first = hashValue;
			(this->node)[bound].value.second = (this->leafNode)[idx++].getHashTx();
		}
	}	

}

vector<Transaction> MerkleTree::findAllTX()
{
	vector<Transaction> ret;
	
		for (int i = 0; i < 100; i++)
		{
			if ((this->leafNode)[i].getTrID() == "")
				break;
			ret.push_back((this->leafNode)[i]);
		}	
	return ret;
}

string MerkleTree::getRootHash()
{
	Node rootNode =(this->node)[0];
	string rootHash = rootNode.value.first + rootNode.value.second;
	return rootHash;
}

int MerkleTree::serialize(TCHAR* buf)
{
	int nodeNum = (this->nodeNum);
	int txNum = (this->txNum);
	short offset = 0;

	int* i = &(nodeNum);
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	i = &(txNum);
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	TCHAR temp[BUFSIZE];
	for (int k = 0; k < txNum; k++)
	{
		Transaction t = (this->leafNode)[k];
		int size = t.serialize(temp);
		int* i = &size;
		memcpy(buf + offset, i, sizeof(int));
		offset += 2;

		memcpy(buf + offset, temp, size * sizeof(TCHAR));
		offset += size/2;
	}

	char* nodeBuf;
	for (int k = 0; k < nodeNum; k++)
	{
		Node n = (this->node)[k];
		string sum = n.value.first + n.value.second;
		nodeBuf = const_cast<char*>(sum.c_str());
		memcpy(buf + offset, nodeBuf, 64 * sizeof(char));
		offset += 32;
	}

	return offset;
}

void MerkleTree::deserialize(TCHAR* buf)
{
	int nodeNum, txNum;
	short offset = 0;
	int* i = &nodeNum;
	memcpy(i, buf + offset, sizeof(int));
	this->nodeNum=nodeNum = *i;
	offset += 2;

	memcpy(i, buf + offset, sizeof(int));
	this->txNum= txNum = *i;
	offset += 2;

	TCHAR temp[BUFSIZE];
	//deserialize tx
	for (int i = 0; i < this->txNum; i++)
	{	
		int size;
		int* p = &size;

		memcpy(p, buf + offset, sizeof(int));
		offset += 2;		

		memcpy(temp, buf + offset, size * sizeof(TCHAR));
		offset += size/ 2;
		(this->leafNode)[i].deserialize(temp);
	}

	//deserialize node
	char left[BUFSIZE];
	char right[BUFSIZE];
	for (int i = 0; i < this->nodeNum; i++)
	{
		memcpy(left, buf + offset, 32 * sizeof(char));
		offset += 16 * sizeof(char);

		memcpy(right, buf + offset, 32 * sizeof(char));
		offset += 16 * sizeof(char);

		((this->node)[i].value.first).resize(32);
		((this->node)[i].value.second).resize(32);
		//(this->node)[i].value.first = left;
		for (int j = 0; j < 32; j++)
		{
			((this->node)[i].value.first)[j] = left[j];
			((this->node)[i].value.second)[j] = right[j];

		}
		//(this->node)[i].value.second = right;
		//memcpy(const_cast<char*>((this->node)[i].value.first.c_str()), left, 256 * sizeof(char));
		//memcpy(const_cast<char*>((this->node)[i].value.second.c_str()), right, 256 * sizeof(char));

	}

	
}


/*----------
BLOCKCHAIN
------------*/
void BlockChain::insertBlock(Block block)
{
	//Genesis Block
	if (this->blockChain.size() == 0)
	{
		block.getHeader().setHashPointer(*(new HashPointer()));
	}
	else
	{
		Block prevBlock = (this->blockChain)[this->blockChain.size() - 1];
		HashPointer* hashPointer = new HashPointer(&prevBlock, prevBlock.getHashRes());
		block.getHeader().setHashPointer(*hashPointer);
	}	
}

Block BlockChain::getLastBlock()
{
	vector<Block> bc = this->blockChain;
	return bc[bc.size() - 1];
}

char* BlockChain::findProductOwner(int identifier)
{
	//TODO : branch가 일어난 경우 더 긴 chain을 찾아야함.
	//block의 개수를 어떻게 알지 -> blockNum을 이용?
	char* temp=NULL;
	return temp;

}

Product BlockChain::findProduct(int identifier)
{
	return Product();
}

void Product::serialize(TCHAR* retBuf)
{
	time_t* t = (time_t*)retBuf;
	*t = this->manufacturedDate; t++;
	
	int* i = (int*)t;
	*i = this->identifier; i++;

	char* c = (char*)i;
	for (int i = 0; i < 50; i++)
	{
		*c = this->modelNo[i];
		c++;
	}
	
}

void Product::deSerialize(TCHAR* retBuf)
{
	time_t* t = (time_t*)retBuf;
	this->manufacturedDate = *t; t++;

	int* i = (int*)t;
	this->identifier = *i; i++;

	char* c = (char*)i;
	for (int i = 0; i < 50; i++)
	{
		this->modelNo[i] = *c;
		c++;
	}
	
}

int Transaction::serialize(TCHAR* buf)
{
	//input,outpit serialize
	int offset = 0;
	memcpy(buf + offset, this->input, 130 * sizeof(char));
	offset += 130 * sizeof(char) / 2;
	memcpy(buf + offset, this->output, 130 * sizeof(char));
	offset += 130 * sizeof(char) / 2;

	//sig serialize
	memcpy(buf + offset, this->sigR,64 * sizeof(char));
	offset += 64 * sizeof(char) / 2;

	memcpy(buf + offset, this->sigS, 64 * sizeof(char));
	offset += 64 * sizeof(char) / 2;

	//trID, hashTx, others serialize
	memcpy(buf + offset, this->trID, 32 * sizeof(char));
	offset += 16 * sizeof(char);
	memcpy(buf + offset, this->hashTx, 32 * sizeof(char));
	offset += 16 * sizeof(char);
	memcpy(buf + offset, this->others, 20 * sizeof(char));
	offset += 10 * sizeof(char);

	//price, tradingDate serialize
	time_t* t = &(this->tradingDate);
	memcpy(buf + offset, t, sizeof(time_t));
	offset += 4;

	int* i = &(this->price);
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	TCHAR proBuf[100];
	this->product.serialize(proBuf);

	memcpy(buf + offset, proBuf,62 * sizeof(TCHAR));
	offset += 62;
	return offset;
	
}

void Transaction::deserialize(TCHAR* buf)
{
	short offset = 0;

	//input,outpit deserialize		
	memcpy(this->input, buf+offset, 130 * sizeof(char));
	offset += 65 * sizeof(char);

	memcpy(this->output, buf + offset, 130 * sizeof(char));
	offset += 65 * sizeof(char);	

	//sig deserialize
	memcpy(this->sigR, buf + offset, 64 * sizeof(char));
	offset += 32 * sizeof(char);

	memcpy(this->sigS, buf + offset, 64 * sizeof(char));
	offset += 32 * sizeof(char);

	//trID, hashTx, others deserialize
	memcpy(this->trID, buf + offset, 32 * sizeof(char));
	offset += 16 * sizeof(char);
	memcpy(this->hashTx, buf + offset, 32 * sizeof(char));
	offset += 16 * sizeof(char);
	memcpy(this->others, buf + offset, 20 * sizeof(char));
	offset += 10 * sizeof(char);

	//price, tradingDate deserialize
	time_t* t = &(this->tradingDate);
	memcpy(t,buf + offset, sizeof(time_t));
	this->tradingDate = *t;
	offset += 4;

	int* i = &(this->price);
	memcpy(i,buf + offset, sizeof(int));
	this->price = *i;
	offset += 2;

	TCHAR proBuf[100];

	memcpy(proBuf, buf + offset,62 * sizeof(TCHAR));
	
	(this->product).deSerialize(proBuf);

	
}

EC_KEY* makeEcKey(const char* priv)
{
	EC_KEY* eckey = NULL;
	EC_POINT* pub_key = NULL;
	const EC_GROUP* group = NULL;

	BIGNUM* res = BN_new();
	BN_CTX* ctx = BN_CTX_new();

	BN_hex2bn(&res, priv);
	eckey = EC_KEY_new_by_curve_name(NID_secp256k1);
	group = EC_KEY_get0_group(eckey);
	pub_key = EC_POINT_new(group);

	EC_KEY_set_private_key(eckey, res);
		
	EC_POINT_mul(group, pub_key, res, NULL, NULL, ctx);
	
	EC_KEY_set_public_key(eckey, pub_key);
	return eckey;
}

void Block::serialize(TCHAR* buf)
{
	int offset = 0;
	//serialize header
	int blockNo = (this->header).getBlockNo();
	int* i = &blockNo;
	
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	char* c = (this->header).getMrklRootHash();
	memcpy(buf + offset, c, 64 * sizeof(char));
	offset += 32 * sizeof(char);

	c = (this->header).getHashPointer()->getPrevHash();
	memcpy(buf + offset, c, 32 * sizeof(char));
	offset += 16 * sizeof(char);

	unsigned int* ui = &(this->header.nonce);
	memcpy(buf + offset, ui, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	//serialize hashRes
	c = (this->hashRes);
	memcpy(buf + offset, c, 32 * sizeof(char));
	offset += 16;

	//serilize merkleTree
	TCHAR temp[BUFSIZE];
	int size = (this->merkleTree)->serialize(temp);
	i = &size;
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	memcpy(buf + offset, temp, size * sizeof(TCHAR));
	offset += size / 2;	
	
}

void Block::deSerialize(TCHAR* buf)
{
	int offset = 0;

	//deserialize header
	int blockNo = (this->header).getBlockNo();
	int* i = &blockNo;

	memcpy(i,buf + offset, sizeof(int));
	offset += 2;
	this->header.setBlockNo(blockNo);

	char* c = (this->header).getMrklRootHash();
	memcpy(c,buf + offset, 64 * sizeof(char));
	offset += 32 * sizeof(char);
	this->header.setMrklRootHash(c);

	c = (this->header).getHashPointer()->getPrevHash();
	memcpy(c,buf + offset, 32 * sizeof(char));
	offset += 16 * sizeof(char);
	
	(this->header.getHashPointer())->setPrevHash(c);

	unsigned int* ui = &(this->header.nonce);
	memcpy(ui, buf + offset, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	this->header.nonce = *ui;

	//deserialize hashRes
	c = (this->hashRes);
	memcpy(c,buf + offset, 32 * sizeof(char));
	offset += 16;
	this->setHashRes(c);

	//deserilize merkleTree	
	memcpy(i, buf + offset, sizeof(int));
	offset += 2;
	int size = *i;

	TCHAR temp[BUFSIZE];
	memcpy(temp, buf + offset, size * sizeof(TCHAR));
	(this->merkleTree)->deserialize(temp);

}
