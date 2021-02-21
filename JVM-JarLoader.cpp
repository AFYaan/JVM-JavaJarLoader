#include "Loader.h"
#include "LoaderUtils.h"

int main(int argsLength, const char* args[])
{
    string jarPath = GetExeDir() + string("\\JVM_TEST.jar");
    string mainMethod("pl/afyaan/Main");
    Loader* loader = new Loader(jarPath.c_str(), mainMethod.c_str());
    loader->Run(args, argsLength);
}