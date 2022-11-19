#pragma once
#include<iostream>
#include<cstdio>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/obj_mac.h>
#include<openssl/bn.h>
#include<openssl/sha.h>
#include<bitset>
#include<functional>
#include<ctime>
#include<vector>
#include<stdlib.h>
#include<string>
#include<stdint.h>
#include<list>
#include<typeinfo>
#include<map>
#include<Windows.h>

using namespace std;

template<class T>
string hashTX(T* t)
{
	unsigned char* tCast = reinterpret_cast<unsigned char*>(t);
	unsigned char* temp=nullptr;
	unsigned char* ret = SHA256(tCast, sizeof(t), temp);
	string s(reinterpret_cast<char*>(ret));
	return s;
}