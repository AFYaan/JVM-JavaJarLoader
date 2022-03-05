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

#include "Loader.h"

#include "LoaderUtils.h"
#include <iostream>

#include "Decryptor.h"

#include "ClassLoader.h"

using namespace std;

Loader::Loader(const string mainClass) {
	this->Loader::Loader(GetCurrentExeName(), mainClass);
}

Loader::Loader(const string jarPath, const string mainClass) {
	this->jarPath = jarPath;
	this->mainClassPath = mainClass;
	this->isInit = false;
	this->jvm = nullptr;
}

void Loader::Init() {
	if (isInit) return;

	jvmMod = LoadLibraryA((GetExeDir() + string("\\bin\\server\\jvm.dll")).c_str());

	if (jvmMod == nullptr) {
		cout << "Error: " << "jvm.dll could not be loaded" << endl;
		exit(EXIT_FAILURE);
	}

	typedef jint(JNICALL* PtrCreateJavaVM)(JavaVM**, void**, void*);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(jvmMod, "JNI_CreateJavaVM");

	JavaVMInitArgs vmArgs;
	JavaVMOption* vmOptions = new JavaVMOption[32];

	string optClassPath = string("-Djava.class.path=") + this->jarPath;
	
	int argLength = 0;
	vmOptions[argLength].optionString = (char*)optClassPath.c_str();
	
	for (const char* arg : jvmArgList) {
		argLength++;
		vmOptions[argLength].optionString = (char*)arg;
	}

	vmArgs.version = JNI_VERSION_1_8;
	vmArgs.nOptions = argLength + 1;
	vmArgs.options = vmOptions;
	vmArgs.ignoreUnrecognized = JNI_FALSE;

	jint vm = ptrCreateJavaVM(&jvm, (void**)&env, &vmArgs);

	if (vm != JNI_OK) {
		cout << "Error: " << "Can't create java vm" << endl;
		exit(EXIT_FAILURE);
	}

	isInit = true;
}

void Loader::SetPassword(const string password) {
	this->password = password;
}

void Loader::Run() {
	Run(NULL, 0);
}

void Loader::Run(const char* args[], int argsLength) {
	Init();

	jclass mainClass = env->FindClass(mainClassPath.c_str());

	if (mainClass == nullptr) {
		cout << "Error: " << "Main class not found" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	if (mainMethod == nullptr) {
		cout << "Error: " << "Main method not found" << endl;
		exit(EXIT_FAILURE);
	}

	const char* newArgs[1024];
	int newLength = 0;

	for (int i = 0; i < argsLength; i++) {
		newArgs[i] = args[i];
		newLength++;
	}

	for (const char* arg : appArgs) {
		newArgs[newLength] = arg;
		newLength++;
	}

	for (int i = 0; i < newLength; i++) {
		cout << "Arg: " << newArgs[i] << endl;
	}
	
	env->CallStaticVoidMethod(mainClass, mainMethod, CharArrayToJavaStringArray(env, newArgs, newLength));
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, bool autoDestroy) {
	RunFromMemory(data, dataSize, NULL, 0, autoDestroy);
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength, bool autoDestroy) {
	Init();

	jclass byteArrayinputStream = env->FindClass("java/io/ByteArrayInputStream");
	jmethodID byteArrayinputStreamConstructor = env->GetMethodID(byteArrayinputStream, "<init>", "([B)V");
	jobject stream = nullptr;
	if (!this->password.empty()) {
		unique_ptr<Decryptor> decryptor(new Decryptor(env, password.c_str()));
		jobject decryptedData = decryptor->Decrypt(CharArrayToJavaByteArray(env, data, dataSize));

		stream = env->NewObject(byteArrayinputStream, byteArrayinputStreamConstructor, decryptedData);
	}
	else {
		stream = env->NewObject(byteArrayinputStream, byteArrayinputStreamConstructor, CharArrayToJavaByteArray(env, data, dataSize));
	}

	unique_ptr<ClassLoader> classLoader(new ClassLoader(env, stream, password));
	classLoader->load();

	jclass mainClass = env->FindClass(mainClassPath.c_str());
	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	const char* newArgs[1024];
	int newLength = 0;

	for (int i = 0; i < argsLength; i++) {
		newArgs[i] = args[i];
		newLength++;
	}

	for (const char* arg : appArgs) {
		newArgs[newLength] = arg;
		newLength++;
	}

	jobjectArray argss = CharArrayToJavaStringArray(env, newArgs, newLength);

	env->CallStaticVoidMethod(mainClass, mainMethod, argss);

	if (autoDestroy) DestroyVM();
}

void Loader::DestroyVM() {
	jvm->DestroyJavaVM();
}

void Loader::SetJVMArgs(const char* args[], int size) {
	jvmArgList.clear();
	for (int i = 0; i < size; i++) {
		jvmArgList.push_back(args[i]);
	}
}

void Loader::AddJVMArg(const char* arg) {
	jvmArgList.push_back(arg);
}

void Loader::AddArg(const char* arg) {
	appArgs.push_back(arg);
}