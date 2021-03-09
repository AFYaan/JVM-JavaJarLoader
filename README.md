# JVM Java Jar Loader
JavaJarLoader runs a jar file contained in a byte array or in a file.

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/27700785ed774aad99d559135d793175)](https://github.com/AFYaan/JVM-JavaJarLoader)

## Run from file usages

### Usage 1

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

int main()
{
    string jarPath = GetExeDir() + string("\\JVM_TEST.jar");
    string mainMethod("pl/afyaan/Main");

    Loader* loader = new Loader(jarPath, mainMethod);
    loader->Run();
}
```

### Usage 2

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

int main(int argsLength, const char* args[])
{
    string jarPath = GetExeDir() + string("\\JVM_TEST.jar");
    string mainMethod("pl/afyaan/Main");

    Loader* loader = new Loader(jarPath, mainMethod);
    loader->Run(args, argsLength);
}
```

### Usage 3

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

int main()
{
    const char* args[] = { "arg1", "arg2" };
    int argsLength = sizeof(args) / sizeof(const char*);

    string jarPath = GetExeDir() + string("\\JVM_TEST.jar");
    string mainMethod("pl/afyaan/Main");

    Loader* loader = new Loader(jarPath, mainMethod);
    loader->Run(args, argsLength);
}
```

## Run from memory usages

### Usage 4

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

const unsigned char rawData[1863] = {
	0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08, 0x08, 0x08, 0x00, 0x6C, 0x2F,
	0x57, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x14, 0x00, 0x04, 0x00, 0x4D, 0x45, 0x54, ...
};

int main()
{
    string mainMethod("pl/afyaan/Main");
    
    //Resource file is a current exe
    Loader* loader = new Loader(mainMethod);
    loader->RunFromMemory(rawData, sizeof(rawData));
}
```

### Usage 5

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

const unsigned char rawData[1863] = {
	0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08, 0x08, 0x08, 0x00, 0x6C, 0x2F,
	0x57, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x14, 0x00, 0x04, 0x00, 0x4D, 0x45, 0x54, ...
};

int main(int argsLength, const char* args[])
{
    string mainMethod("pl/afyaan/Main");
    
    //Resource file is a current exe
    Loader* loader = new Loader(mainMethod);
    loader->RunFromMemory(rawData, sizeof(rawData), args, argsLength);
}
```

### Usage 6

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

const unsigned char rawData[1863] = {
	0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08, 0x08, 0x08, 0x00, 0x6C, 0x2F,
	0x57, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x14, 0x00, 0x04, 0x00, 0x4D, 0x45, 0x54, ...
};

int main(int argsLength, const char* args[])
{
    //Resources file (.jar, .exe, .zip)
    string resPath = GetExeDir() + string("\\JVM_TEST.jar");

    string mainMethod("pl/afyaan/Main");
    Loader* loader = new Loader(resPath, mainMethod);
    loader->RunFromMemory(rawData, sizeof(rawData), args, argsLength);
}
```


### Usage 7

You need to generate the necessary files with [JJL-Encryptor](https://github.com/AFYaan/JJL-Encryptor)

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

//AES encrypted data
const unsigned char rawData[2416] = {
	0xA5, 0xA0, 0xD6, 0x0F, 0xD0, 0xD4, 0xE4, 0x07, 0x45, 0xFB, 0xB8, 0x35,
	0x4E, 0x3B, 0x0C, 0xD0, 0xE0, 0x29, 0x0F, 0xDB, 0x56, 0xA0, 0x1E, 0x6F, 
	0x4E, 0x04, 0x7E, 0xD6, 0x47, 0xE1, 0xD8, 0x59, 0xF3, ...
};

int main(int argsLength, const char* args[])
{
    //Resources file (.jar, .exe, .zip)
    string resPath = GetExeDir() + string("\\JVM_TEST-resources.jar");

    string mainMethod("pl/afyaan/Main");
    unique_ptr<Loader> loader(new Loader(resPath, mainMethod));
    
    //causes automatic AES decryption 
    loader->SetPassword("testpassword");
    
    loader->RunFromMemory(rawData, sizeof(rawData), args, argsLength);
}
```
