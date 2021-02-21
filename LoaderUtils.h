#pragma once
#include <windows.h>
#include <string>
#include "jni.h"

using namespace std;

string getExeDir();

jobjectArray charArrayToJavaArray(JNIEnv* env, const char* data[], int size);