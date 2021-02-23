#pragma once
#include <jni.h>

class ClassLoader
{
private:
	JNIEnv* env;
	jobject classes;
	jobject classLoader;

public:
	ClassLoader(JNIEnv* env, jobject stream);
	jobject loadClass(jstring name);

private:
	jbyteArray readClass(jobject stream);
};

