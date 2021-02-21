# JVM Java Jar Loader

## Usage 1

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

## Usage 2

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

## Usage 2

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
