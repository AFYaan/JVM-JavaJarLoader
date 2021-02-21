#pragma once
#include <Windows.h>
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
	Loader(const char* jarPath, const char* mainMethod);

public:
	void Run();
	void Run(const char* args[], int size);

private:
	void Init();

};

