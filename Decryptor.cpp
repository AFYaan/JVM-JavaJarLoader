#include "Decryptor.h"

Decryptor::Decryptor(JNIEnv* env, const char* key) {
	this->env = env;
	this->key = env->NewStringUTF(key);

	SetKey(this->key);
	Init();
}

void Decryptor::Init() {
	jclass cipherClass = env->FindClass("javax/crypto/Cipher");
	jmethodID getInstance = env->GetStaticMethodID(cipherClass, "getInstance", "(Ljava/lang/String;)Ljavax/crypto/Cipher;");
	jmethodID init = env->GetMethodID(cipherClass, "init", "(ILjava/security/Key;)V");

	this->cipher = env->CallStaticObjectMethod(cipherClass, getInstance, env->NewStringUTF("AES/ECB/PKCS5PADDING"));
	env->CallVoidMethod(cipher, init, 2, this->secretKey);

	if (cipher == nullptr) {
		cout << "Error: " << "Main class not found" << endl;
		exit(EXIT_FAILURE);
	}
}

void Decryptor::SetKey(jstring key) {
	jclass stringClass = env->FindClass("java/lang/String");
	jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/nio/charset/Charset;)[B");

	jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
	jmethodID getInstance = env->GetStaticMethodID(messageDigestClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
	jmethodID digest = env->GetMethodID(messageDigestClass, "digest", "([B)[B");

	jclass arraysClass = env->FindClass("java/util/Arrays");
	jmethodID copyOf = env->GetStaticMethodID(arraysClass, "copyOf", "([BI)[B");

	jclass charsetClass = env->FindClass("java/nio/charset/Charset");
	jmethodID forName = env->GetStaticMethodID(charsetClass, "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");

	jobject UTF_8 = env->CallStaticObjectMethod(charsetClass, forName, env->NewStringUTF("UTF-8"));
	jbyteArray keyData = (jbyteArray)env->CallObjectMethod(key, getBytes, UTF_8);

	jobject sha = env->CallStaticObjectMethod(messageDigestClass, getInstance, env->NewStringUTF("SHA-1"));
	keyData = (jbyteArray)env->CallObjectMethod(sha, digest, keyData);
	keyData = (jbyteArray)env->CallStaticObjectMethod(arraysClass, copyOf, keyData, 16);

	jclass secretKeySpecClass = env->FindClass("javax/crypto/spec/SecretKeySpec");
	jmethodID secretKeySpecConstructor = env->GetMethodID(secretKeySpecClass, "<init>", "([BLjava/lang/String;)V");

	this->secretKey = env->NewObject(secretKeySpecClass, secretKeySpecConstructor, keyData, env->NewStringUTF("AES"));
}

jbyteArray Decryptor::Decrypt(jbyteArray data)
{
	jclass cipherClass = env->FindClass("javax/crypto/Cipher");
	jmethodID doFinal = env->GetMethodID(cipherClass, "doFinal", "([B)[B");

	return (jbyteArray)env->CallObjectMethod(cipher, doFinal, data);
}
