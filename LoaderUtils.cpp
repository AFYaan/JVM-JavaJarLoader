#include "LoaderUtils.h"
#include <iostream>

string GetExeDir() {
    char result[MAX_PATH];
    string dir = string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
    size_t last = dir.find_last_of("\\");

    return dir.substr(0, last);
}

jobjectArray charArrayToJavaStringArray(JNIEnv* env, const char* data[], int size) {
    jobjectArray array = env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for (int i = 0; i < size; i++) env->SetObjectArrayElement(array, i, env->NewStringUTF(data[i]));

    return array;
}

jbyteArray charArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len) {
    jbyteArray array = env->NewByteArray(len);
    jbyte* a = new jbyte[len];

    for (int i = 0; i < len; i++) a[i] = (jbyte)((unsigned char)data[i]);

    env->SetByteArrayRegion(array, 0, len, a);

    return array;
}

string replaceAll(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}