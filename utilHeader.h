#pragma once
#include<iostream>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/obj_mac.h>
#include<openssl/bn.h>
#include<openssl/sha.h>
#include<ctime>
#include<vector>
#include<stdlib.h>
#include<string>
#include<stdint.h>
#include<list>
#include<typeinfo>


using namespace std;

/*SHA256을 이용한 HASH*/
template<typename T>
unsigned char* hashT(T* t)
{
	SHA256_CTX c;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	unsigned char* ret = hash;
	SHA256_Init(&c);
	SHA256_Update(&c, t, sizeof(*t));
	SHA256_Final(ret, &c);
	return ret;
}
