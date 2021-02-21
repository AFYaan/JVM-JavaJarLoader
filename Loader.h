#pragma once
#include <Windows.h>
#include "jni.h"

class Loader
{
private:
	HMODULE jvmMod;

	JavaVM* jvm;
	JNIEnv* env;

public:
	Loader();

public:
	void Run();
	void Run(const char* args[], int size);

private:
	void Init();

};

