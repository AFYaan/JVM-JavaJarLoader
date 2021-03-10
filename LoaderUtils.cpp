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