#include"utilHeader.h"
#include"data.h"
/*
Block

각각의 block은 Header와 나머지부분으로 이루어져있다
Header- blockNo
		직전 block에 대한 hash pointer
		nonce
		mrkl tree의 hash값

나머지 - mrkl tree (coinbase tx)는 존재 X
*/

class Block 
{
private:
	int blockNo;
	string prev;
	uint32_t nonce;
	string mrkl_root;
	MerkleTree mrkl_tree;

public:

};