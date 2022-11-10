#pragma once
#include<iostream>
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

using namespace std;

/*SHA256을 이용한 HASH*/
template<typename T>
unsigned char* hashT(T* t)
{
	unsigned char* hashRes=nullptr;
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, t, sizeof(t));
	SHA256_Final(hashRes, &ctx);
	return hashRes;
}
