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

#include "ClassLoader.h"
#include <iostream>
#include "Decryptor.h"

using namespace std;

ClassLoader::ClassLoader(JNIEnv* env, jobject stream, string password) {
	this->env = env;
	this->stream = stream;
	this->classes = nullptr;
	this->classLoader = nullptr;
	this->password = password;
}

void ClassLoader::load() {
	jclass hashMap = env->FindClass("java/util/HashMap");
	jmethodID hashMapConstructor = env->GetMethodID(hashMap, "<init>", "()V");
	classes = env->NewObject(hashMap, hashMapConstructor);
	jmethodID put = env->GetMethodID(hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID getSystemClassLoader = env->GetStaticMethodID(classLoaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

	classLoader = env->CallStaticObjectMethod(classLoaderClass, getSystemClassLoader);

	jclass jarInputStream = env->FindClass("java/util/jar/JarInputStream");
	jmethodID jarInputStreamConstructor = env->GetMethodID(jarInputStream, "<init>", "(Ljava/io/InputStream;)V");
	jobject jis = env->NewObject(jarInputStream, jarInputStreamConstructor, stream);
	jmethodID getNextJarEntry = env->GetMethodID(jarInputStream, "getNextJarEntry", "()Ljava/util/jar/JarEntry;");


	jclass jarEntry = env->FindClass("java/util/jar/JarEntry");
	jmethodID getName = env->GetMethodID(jarEntry, "getName", "()Ljava/lang/String;");

	jclass stringClass = env->FindClass("java/lang/String");
	jmethodID endsWith = env->GetMethodID(stringClass, "endsWith", "(Ljava/lang/String;)Z");
	jmethodID replaceAll = env->GetMethodID(stringClass, "replaceAll", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	jclass zipInputStream = env->FindClass("java/util/zip/ZipInputStream");
	jmethodID zipInputStreamClose = env->GetMethodID(zipInputStream, "close", "()V");

	jclass inputStream = env->FindClass("java/io/InputStream");
	jmethodID inputStreamClose = env->GetMethodID(inputStream, "close", "()V");

	jobject je = NULL;
	jstring entryName = NULL;

	jstring classNames[64000] = { nullptr };
	bool definedEntries[64000] = { false };

	int entryCount = 0;
	while ((je = env->CallObjectMethod(jis, getNextJarEntry)) != NULL) {
		entryName = (jstring)env->CallObjectMethod(je, getName);
		jboolean name = env->CallBooleanMethod(env->CallObjectMethod(je, getName), endsWith, env->NewStringUTF(".class"));

		if (name) {
			jbyteArray classBytes = readClass(jis);
			jstring canonicalName = (jstring)env->CallObjectMethod(
				env->CallObjectMethod(entryName, replaceAll, env->NewStringUTF("/"), env->NewStringUTF(".")), replaceAll,
				env->NewStringUTF(".class"), env->NewStringUTF(""));
			classNames[entryCount] = canonicalName;
			env->CallObjectMethod(classes, put, canonicalName, classBytes);
			entryCount++;
		}
	}

	env->CallVoidMethod(jis, zipInputStreamClose);
	env->CallVoidMethod(stream, inputStreamClose);

	int defined = 0;
	while (true) {
		int loopCount = 0;
		if (defined >= entryCount) break;
		while (true) {
			if (loopCount >= entryCount) break;
			if (definedEntries[loopCount] != true) {
				jstring className = classNames[loopCount];
				jobject load = loadClass(className);

				if (load) {
					definedEntries[loopCount] = true;
					defined++;
				}
			}
			loopCount++;
		}
	}
}

jbyteArray ClassLoader::readClass(jobject stream) {
	jclass byteArrayOutputStream = env->FindClass("java/io/ByteArrayOutputStream");
	jmethodID byteArrayOutputStreamConstructor = env->GetMethodID(byteArrayOutputStream, "<init>", "()V");
	jobject baos = env->NewObject(byteArrayOutputStream, byteArrayOutputStreamConstructor);
	jmethodID write = env->GetMethodID(byteArrayOutputStream, "write", "(I)V");
	jmethodID toByteArray = env->GetMethodID(byteArrayOutputStream, "toByteArray", "()[B");

	jclass inputStream = env->FindClass("java/io/InputStream");
	jmethodID read = env->GetMethodID(inputStream, "read", "()I");

	while (true) {
		int qwe = env->CallIntMethod(stream, read);

		if (qwe == -1) break;

		env->CallVoidMethod(baos, write, qwe);
	}

	jbyteArray classBytes = (jbyteArray)env->CallObjectMethod(baos, toByteArray);

	
	return classBytes;
	
}

jobject ClassLoader::loadClass(jstring name) {
	jclass hashMap = env->FindClass("java/util/HashMap");
	jmethodID get = env->GetMethodID(hashMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID defineClass = env->GetMethodID(classLoaderClass, "defineClass", "(Ljava/lang/String;[BII)Ljava/lang/Class;");

	jbyteArray classBytes = (jbyteArray)env->CallObjectMethod(classes, get, name);

	return env->CallObjectMethod(classLoader, defineClass, name, classBytes, 0, (int)env->GetArrayLength(classBytes));
}
