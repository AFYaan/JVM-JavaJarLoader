/*
 * Copyright 2021 AFYaan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License atv
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

