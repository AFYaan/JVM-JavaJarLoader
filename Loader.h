#pragma once
#include <Windows.h>
#include <iostream>
#include <list>
#include "jni.h"
#include <string>

using namespace std;

class Loader
{
private:
	string jarPath;
	string mainClassPath;
	string password;

	HMODULE jvmMod;

	JavaVM* jvm;
	JNIEnv* env;

	list<const char*> jvmArgList;
	list<const char*> appArgs;

	bool isInit;

public:
	Loader(string mainMethod);
	Loader(string jarPath, string mainMethod);

public:
	void Run();
	void Run(const char* args[], int argsLength);
	void RunFromMemory(const unsigned char data[], int dataSize, bool autoDestroy = true);
	void RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength, bool autoDestroy = true);
	void DestroyVM();
	void SetJVMArgs(const char* args[], int size);
	void AddJVMArg(const char* arg);
	void AddArg(const char* arg);
	void SetPassword(string key);

private:
	void Init();

};

