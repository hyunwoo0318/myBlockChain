#include "data.h"

/*----------
	HEADER
------------*/

Header::Header(long blockNo, unsigned char* prev, uint32_t nonce,
	unsigned char* mrkl_root_hash)
{
	this->blockNo = blockNo;
	this->prev = prev;
	this->nonce = nonce;
	this->mrkl_root_hash = mrkl_root_hash;
}

/*----------
	MERKLE TREE
------------*/

void MerkleTree::makeMerkleTree(vector<Transaction> tx)
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

			string hashLeftValue = hashTX(leftValue);
			string hashRightValue = hashTX(rightValue);

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

			string hashLeftValue = hashTX(leftValue);
			string hashRightValue = hashTX(rightValue);

			(*this->node)[i].value.first = hashLeftValue;
			(*this->node)[i].value.second = hashRightValue;
		}

		//자식이 하나는 tx, 하나는 노드인경우
		{
			Node boundChild = (*this->node)[2 * bound + 1];
			string childValue = boundChild.value.first + boundChild.value.second;
			string hashValue = hashTX(childValue);
			(*this->node)[bound].value.first = hashValue;
			(*this->node)[bound].value.second = (*this->leafNode)[idx++].getHashTx();
		}
	}	

}

vector<Transaction> MerkleTree::findAllTX()
{
	//TODO : 구현
}

/*----------
	BLOCKCHAIN
------------*/
void BlockChain::insertBlock(Block prevBlock, Block curBlock)
{
}

EC_KEY* BlockChain::findProductOwner(int identifier)
{
	return nullptr;
}

Product BlockChain::findProduct(int identifier)
{
	return Product();
}
