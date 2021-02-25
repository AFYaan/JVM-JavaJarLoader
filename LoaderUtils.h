#pragma once
#include <windows.h>
#include <string>
#include "jni.h"

using namespace std;

string GetExeDir();

string GetCurrentExeName();

jobjectArray CharArrayToJavaStringArray(JNIEnv* env, const char* data[], int size);

jbyteArray CharArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len);