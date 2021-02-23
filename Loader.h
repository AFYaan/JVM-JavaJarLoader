#pragma once
#include <Windows.h>
#include <iostream>
#include "jni.h"

class Loader
{
private:
	const char* jarPath;
	const char* mainMethod;

	HMODULE jvmMod;

	JavaVM* jvm;
	JNIEnv* env;

public:
	Loader(const char* mainMethod);
	Loader(const char* jarPath, const char* mainMethod);

public:
	void Run();
	void Run(const char* args[], int size);
	void RunFromMemory(const unsigned char data[], int size);
	void RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength);

private:
	void Init();

};

