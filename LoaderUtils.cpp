#include "LoaderUtils.h"

string GetExeDir() {
    char result[MAX_PATH];
    string dir = string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
    size_t last = dir.find_last_of("\\");

    return dir.substr(0, last);
}

string GetCurrentExeName() {
    char name[MAX_PATH];
    GetModuleFileNameA(nullptr, name, MAX_PATH);
    return name;
}

jobjectArray CharArrayToJavaStringArray(JNIEnv* env, const char* data[], int size) {
    jobjectArray array = env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for (int i = 0; i < size; i++) env->SetObjectArrayElement(array, i, env->NewStringUTF(data[i]));

    return array;
}

jbyteArray CharArrayToJavaByteArray(JNIEnv* env, const unsigned char data[], int len) {
    jbyteArray array = env->NewByteArray(len);
    jbyte* bytes = new jbyte[len];

    for (int i = 0; i < len; i++) bytes[i] = (jbyte)((unsigned char)data[i]);

    env->SetByteArrayRegion(array, 0, len, bytes);

    return array;
}