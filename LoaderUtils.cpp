#include "LoaderUtils.h"
#include <iostream>

string getExeDir() {
    char result[MAX_PATH];
    string dir = string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
    size_t last = dir.find_last_of("\\");

    return dir.substr(0, last);
}

jobjectArray charArrayToJavaArray(JNIEnv*env, const char* data[], int size) {
	jobjectArray array = env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

	for (int i = 0; i < size; i++) env->SetObjectArrayElement(array, i, env->NewStringUTF(data[i]));

	return array;
}