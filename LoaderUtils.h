#pragma once
#include <windows.h>
#include <string>
#include "jni.h"

using namespace std;

string GetExeDir();

jobjectArray charArrayToJavaStringArray(JNIEnv* env, const char* data[], int size);

jbyteArray charArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len);

string replaceAll(string str, const string& from, const string& to);