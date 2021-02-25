# JVM Java Jar Loader

## Run from file usages

### Usage 1

```cplusplus
#include "Loader.h"
#include "LoaderUtils.h"

int main()
{
    string jarPath = GetExeDir() + string("\\JVM_TEST.jar");
    string mainMethod("pl/afyaan/Main");

    Loader* loader = new Loader(jarPath.c_str(), mainMethod.c_str());
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

    Loader* loader = new Loader(jarPath.c_str(), mainMethod.c_str());
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

    Loader* loader = new Loader(jarPath.c_str(), mainMethod.c_str());
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
    Loader* loader = new Loader(mainMethod.c_str());
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
    Loader* loader = new Loader(mainMethod.c_str());
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
    Loader* loader = new Loader(resPath.c_str(), mainMethod.c_str());
    loader->RunFromMemory(rawData, sizeof(rawData), args, argsLength);
}
```
