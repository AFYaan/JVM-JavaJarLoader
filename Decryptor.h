#pragma once
#include <iostream>
#include <string>
#include "LoaderUtils.h"
#include <jni.h>

using namespace std;

class Decryptor
{
private:
	JNIEnv* env;

	jstring key;
	jobject secretKey;
	jobject cipher;

public:
	Decryptor(JNIEnv* env, const char* key);
	jbyteArray Decrypt(jbyteArray data);

private:
	void Init();
	void SetKey(jstring key);
};

