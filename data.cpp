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
//
//MerkleTree MerkleTree::insertTX(Transaction tx)
//{
//	return MerkleTree();
//}

vector<Transaction> MerkleTree::findAllTX()
{
	return vector<Transaction>();
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
