#include "ClassLoader.h"
#include <iostream>

using namespace std;

ClassLoader::ClassLoader(JNIEnv* env, jobject stream) {
	this->env = env;
	jclass Class = env->FindClass("java/lang/Class");
	jmethodID getClassLoader = env->GetMethodID(Class, "getClassLoader", "()Ljava/lang/ClassLoader;");

	jclass mainClass = env->FindClass("pl/afyaan/Main");
	jobject mainClassLoader = env->CallObjectMethod(mainClass, getClassLoader);

	jclass hashMap = env->FindClass("java/util/HashMap");
	jmethodID hashMapConstructor = env->GetMethodID(hashMap, "<init>", "()V");
	classes = env->NewObject(hashMap, hashMapConstructor);
	jmethodID put = env->GetMethodID(hashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID get = env->GetMethodID(hashMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	jclass classLoaderClassSuper = env->GetSuperclass(env->FindClass("java/lang/ClassLoader"));
	jmethodID classLoaderConstructor = env->GetMethodID(classLoaderClass, "<init>", "(Ljava/lang/ClassLoader;)V");
	jmethodID getSystemClassLoader = env->GetStaticMethodID(classLoaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

	classLoader = env->NewGlobalRef(env->CallStaticObjectMethod(classLoaderClass, getSystemClassLoader));

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

	while ((je = env->CallObjectMethod(jis, getNextJarEntry)) != NULL) {
		entryName = (jstring)env->CallObjectMethod(je, getName);

		jboolean name = env->CallBooleanMethod(env->CallObjectMethod(je, getName), endsWith, env->NewStringUTF(".class"));

		if (name) {
			jbyteArray classBytes = readClass(jis);

			jstring cononicalName = (jstring)env->CallObjectMethod(
				env->CallObjectMethod(entryName, replaceAll, env->NewStringUTF("/"), env->NewStringUTF(".")), replaceAll,
				env->NewStringUTF(".class"), env->NewStringUTF(""));

			jobject test = env->CallObjectMethod(classes, put, cononicalName, classBytes);
		}
	}

	env->CallVoidMethod(jis, zipInputStreamClose);
	env->CallVoidMethod(stream, inputStreamClose);
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

	jbyteArray test = (jbyteArray)env->CallObjectMethod(baos, toByteArray);

	return test;
}

jobject ClassLoader::loadClass(jstring name) {
	jclass hashMap = env->FindClass("java/util/HashMap");
	jmethodID get = env->GetMethodID(hashMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID defineClass = env->GetMethodID(classLoaderClass, "defineClass", "(Ljava/lang/String;[BII)Ljava/lang/Class;");

	jbyteArray classBytes = (jbyteArray)env->CallObjectMethod(classes, get, name);

	return env->CallObjectMethod(classLoader, defineClass, name, classBytes, 0, (int)env->GetArrayLength(classBytes));
}
