#include "Loader.h"

#include "LoaderUtils.h"
#include <iostream>

#include "ClassLoader.h"

using namespace std;

Loader::Loader(const char* mainMethod) {
	string exePath = GetCurrentExeName();
	this->jarPath = exePath.c_str();
	this->mainMethod = mainMethod;
	Init();
}

Loader::Loader(const char* jarPath, const char* mainMethod) {
	this->jarPath = jarPath;
	this->mainMethod = mainMethod;
	Init();
}

void Loader::Init() {
	jvmMod = LoadLibraryA((GetExeDir() + string("\\bin\\server\\jvm.dll")).c_str());

	if (jvmMod == nullptr) {
		cout << "Error: " << "jvm.dll could not be loaded" << endl;
		exit(EXIT_FAILURE);
	}

	typedef jint(JNICALL* PtrCreateJavaVM)(JavaVM**, void**, void*);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(jvmMod, "JNI_CreateJavaVM");

	JavaVMInitArgs vmArgs;
	JavaVMOption* vmOptions = new JavaVMOption[1];

	string optClassPath(string("-Djava.class.path=") + string(jarPath));

	vmOptions[0].optionString = (char*)optClassPath.c_str();
	vmArgs.version = JNI_VERSION_1_8;
	vmArgs.nOptions = 1;
	vmArgs.options = vmOptions;
	vmArgs.ignoreUnrecognized = JNI_FALSE;

	jint vm = ptrCreateJavaVM(&jvm, (void**)&env, &vmArgs);

	if (vm != JNI_OK) {
		cout << "Error: " << "Can't create java vm" << endl;
		exit(EXIT_FAILURE);
	}
}

void Loader::Run() {
	Run(NULL, 0);
}

void Loader::Run(const char* args[], int size) {
	jclass mainClass = env->FindClass(mainMethod);

	if (mainClass == nullptr) {
		cout << "Error: " << "Main class not found" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	if (mainMethod == nullptr) {
		cout << "Error: " << "Main method not found" << endl;
		exit(EXIT_FAILURE);
	}

	env->CallStaticVoidMethod(mainClass, mainMethod, CharArrayToJavaStringArray(env, args, size));
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, bool autoDestroy) {
	RunFromMemory(data, dataSize, NULL, 0, autoDestroy);
}

void Loader::RunFromMemory(const unsigned char data[], int dataSize, const char* args[], int argsLength, bool autoDestroy) {
	jclass byteArrayinputStream = env->FindClass("java/io/ByteArrayInputStream");
	jmethodID byteArrayinputStreamConstructor = env->GetMethodID(byteArrayinputStream, "<init>", "([B)V");
	jobject stream = env->NewObject(byteArrayinputStream, byteArrayinputStreamConstructor, CharArrayToJavaByteArray(env, data, dataSize));

	ClassLoader* classLoader = new ClassLoader(env, stream);
	classLoader->load();

	jclass mainClass = env->FindClass(mainMethod);
	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");
	jobjectArray argss = CharArrayToJavaStringArray(env, args, argsLength);

	env->CallStaticVoidMethod(mainClass, mainMethod, argss);

	if (autoDestroy) DestroyVM();
}

void Loader::DestroyVM() {
	jvm->DestroyJavaVM();
}
