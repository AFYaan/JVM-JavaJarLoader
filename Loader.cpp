#include "Loader.h"

#include "LoaderUtils.h"
#include <iostream>

using namespace std;

Loader::Loader(){
	Init();
}

void Loader::Init() {
	jvmMod = LoadLibraryA((getExeDir() + string("\\bin\\server\\jvm.dll")).c_str());

	if (jvmMod == nullptr) { 
		cout << "Error: " << "jvm.dll could not be loaded" << endl;
		exit(EXIT_FAILURE); 
	}

	typedef jint(JNICALL* PtrCreateJavaVM)(JavaVM**, void**, void*);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(jvmMod, "JNI_CreateJavaVM");
	JavaVMInitArgs vmArgs;
	JavaVMOption* vmOptions = new JavaVMOption[1];

	string optClassPath(string("-Djava.class.path=") + getExeDir() + string("\\JVM_TEST.jar"));

	vmOptions[0].optionString = (char*)optClassPath.c_str();
	vmArgs.version = JNI_VERSION_1_8;
	vmArgs.nOptions = 1;
	vmArgs.options = vmOptions;
	vmArgs.ignoreUnrecognized = JNI_FALSE;

	jint vm = ptrCreateJavaVM(&jvm, (void**)&env, &vmArgs);

	if (vm != JNI_OK) { 
		cout << "Error: " << "Can't create java vm " << endl;
		exit(EXIT_FAILURE); 
	}
}

void Loader::Run() {
	Run(NULL, 0);
}

void Loader::Run(const char* args[], int size) {
	jclass mainClass = env->FindClass("pl/afyaan/Main");

	if (mainClass == nullptr) {
		cout << "Error: " << "Main class not found" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");

	if (mainMethod == nullptr) {
		cout << "Error: " << "Main method not found" << endl;
		exit(EXIT_FAILURE);
	}

	env->CallStaticVoidMethod(mainClass, mainMethod, charArrayToJavaArray(env, args, size));
	jvm->DestroyJavaVM();
}
