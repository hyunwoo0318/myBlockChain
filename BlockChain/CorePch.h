#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)

#define BUFSIZE 1024
#define PRIV0 "7AC4FB117FAF5C7B9BE0C94C714F13F16F34DDB43F665162DD638D5401D72568"
#define PRIV1 "47F9253C0778B67CBB98B3A6FDBF25116D650874F5C4FD10B30B82AB3EFC6B5B"
#define PRIV2 "103B5D5DD0824CEC4E77375B123B7042E9A573A66B4F3DA9214AE0C65333FF5A"
#define PRIV3 "9BB3FE5A236A7DEF216C88623D58EB41C8F813E9D7A00C508C0949A0F3097EBB"

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
#include<thread>
#include<mutex>
#include<tchar.h>
#include <openssl/pem.h>
#include"PipeName.h"

using namespace std;

#include"IPC.h"

extern EC_KEY* a;
extern EC_KEY* b;
extern EC_KEY* a;
extern EC_KEY* a;


template<class T>
string hashTX(T* t)
{
	unsigned char* tCast = reinterpret_cast<unsigned char*>(t);
	unsigned char* temp = nullptr;
	unsigned char* ret = SHA256(tCast, sizeof(t), temp);
	string s(reinterpret_cast<char*>(ret));
	return s;
}

