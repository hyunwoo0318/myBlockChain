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

EC_KEY* BlockChain::findProductOwner(int identifier)
{
	//TODO : branch가 일어난 경우 더 긴 chain을 찾아야함.
	//block의 개수를 어떻게 알지 -> blockNum을 이용?
	EC_KEY* temp=NULL;
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
	int pos = 0;
	EC_KEY* input = this->input;
	EC_KEY* output = this->output;
	ECDSA_SIG* sig = this->sig;
	TCHAR* cInput = reinterpret_cast<TCHAR*>(input);
	TCHAR* cOutput = reinterpret_cast<TCHAR*>(output);
	TCHAR* cSig = reinterpret_cast<TCHAR*>(sig);
	
	memcpy(buf, cInput, 32);
	memcpy(buf + 32, cOutput, 32);
	memcpy(buf + 64, cSig, 72);

	int* i = (int*)(buf + 136);
	*i = this->price; i++;

	time_t* t = (time_t*)i;
	*t = this->tradingDate; t++;

	char* c = (char*)t;
	for (int i = 0; i < 256; i++)
	{
		*c = this->trID[i];
		c++;
	}
	for (int i = 0; i < 256; i++)
	{
		*c = this->hashTx[i];
		c++;
	}
	for (int i = 0; i < 50; i++)
	{
		*c = this->others[i];
		c++;
	}	
}