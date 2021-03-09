#pragma once
#include <jni.h>
#include <string>

using namespace std;

class ClassLoader
{
private:
	JNIEnv* env;
	jobject classes;
	jobject classLoader;
	jobject stream;
	string password;

public:
	ClassLoader(JNIEnv* env, jobject stream, string password = string(""));
	void load();

private:
	jbyteArray readClass(jobject stream);
	jobject loadClass(jstring name);
};