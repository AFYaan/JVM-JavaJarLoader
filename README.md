# JVM Java Jar Loader

## Usage 1

```cplusplus
   #include "Loader.h"
   
   int main(int argsLength, const char* args[])
   {
       Loader* loader = new Loader();
       loader->Run(args, argsLength);
   }
```

## Usage 2

```cplusplus
   #include "Loader.h"
   
   int main()
   {
       Loader* loader = new Loader();
       loader->Run();
   }
```
