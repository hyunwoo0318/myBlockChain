#include "pch.h"
#include"data.h"

/*----------
	HEADER
------------*/

Header::Header(long blockNo, HashPointer prev, uint32_t nonce,
	string mrklRootHash)
{
	this->blockNo = blockNo;
	this->prev = prev;
	this->nonce = nonce;
	this->mrklRootHash = mrklRootHash;
}

/*----------
	MERKLE TREE
------------*/

void MerkleTree::makeMerkleTree(const vector<Transaction> &tx)
{
	//tx의 개수
	int txNum = tx.size();
	//노드의 개수 -> 포화이진트리의 성질
	int nodeNum = txNum - 1;	

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
			(*this->node)[i].value.first = (*this->leafNode)[idx++].getHashTx();
			(*this->node)[i].value.second = (*this->leafNode)[idx++].getHashTx();
		}

		//자식이 둘다 노드인경우
		for (int i = 0; i < bound; i++)
		{			
			Node leftChild = (*this->node)[2 * i + 1];
			Node rightChild = (*this->node)[2 * i + 2];

			//각 노드의 값을 더해서 hash를함
			string leftValue = leftChild.value.first + leftChild.value.second;
			string rightValue = rightChild.value.first + rightChild.value.second;

			string hashLeftValue = hashTX(&leftValue);
			string hashRightValue = hashTX(&rightValue);

			(*this->node)[i].value.first = hashLeftValue;
			(*this->node)[i].value.second = hashRightValue;
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
			(*this->node)[i].value.first = (*this->leafNode)[idx++].getHashTx();
			(*this->node)[i].value.second = (*this->leafNode)[idx++].getHashTx();
		}

		//자식이 둘다 노드인경우
		for (int i = 0; i < bound; i++)
		{
			Node leftChild = (*this->node)[2 * i + 1];
			Node rightChild = (*this->node)[2 * i + 2];

			//각 노드의 값을 더해서 hash를함
			string leftValue = leftChild.value.first + leftChild.value.second;
			string rightValue = rightChild.value.first + rightChild.value.second;

			string hashLeftValue = hashTX(&leftValue);
			string hashRightValue = hashTX(&rightValue);

			(*this->node)[i].value.first = hashLeftValue;
			(*this->node)[i].value.second = hashRightValue;
		}

		//자식이 하나는 tx, 하나는 노드인경우
		{
			Node boundChild = (*this->node)[2 * bound + 1];
			string childValue = boundChild.value.first + boundChild.value.second;
			string hashValue = hashTX(&childValue);
			(*this->node)[bound].value.first = hashValue;
			(*this->node)[bound].value.second = (*this->leafNode)[idx++].getHashTx();
		}
	}	

}

vector<Transaction> MerkleTree::findAllTX()
{
	vector<Transaction> ret;
	
		for (int i = 0; i < 100; i++)
		{
			if ((*this->leafNode)[i].getTrID() == "")
				break;
			ret.push_back((*this->leafNode)[i]);
		}	
	return ret;
}

string MerkleTree::getRootHash()
{
	Node rootNode =(*this->node)[0];
	string rootHash = rootNode.value.first + rootNode.value.second;
	return rootHash;
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

void Transaction::serialize(TCHAR* buf)
{
	//input,outpit serialize
	short offset = 0;
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
	memcpy(buf + offset, this->trID, 256 * sizeof(char));
	offset += 128 * sizeof(char);
	memcpy(buf + offset, this->hashTx, 256 * sizeof(char));
	offset += 128 * sizeof(char);
	memcpy(buf + offset, this->others, 50 * sizeof(char));
	offset += 25 * sizeof(char);

	//price, tradingDate serialize
	time_t* t = &(this->tradingDate);
	memcpy(buf + offset, t, sizeof(time_t));
	offset += 4;

	int* i = &(this->price);
	memcpy(buf + offset, i, sizeof(int));
	offset += 2;

	/*TCHAR product[20];
	this->product.serialize(product);

	memcpy(buf + offset, product,2 * sizeof(TCHAR));*/
	
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
	memcpy(this->trID, buf + offset, 256 * sizeof(char));
	offset += 128 * sizeof(char);
	memcpy(this->hashTx, buf + offset, 256 * sizeof(char));
	offset += 128 * sizeof(char);
	memcpy(this->others, buf + offset, 50 * sizeof(char));
	offset += 25 * sizeof(char);

	//price, tradingDate deserialize
	time_t* t = &(this->tradingDate);
	memcpy(t,buf + offset, sizeof(time_t));
	this->tradingDate = *t;
	offset += 8;

	int* i = &(this->price);
	memcpy(i,buf + offset, sizeof(int));
	this->price = *i;
	offset += 4;

	//TCHAR* product = nullptr;
	//memcpy(product,buf + offset, 2);
	//offset += 2;

	//this->product.deSerialize(product);
	
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
