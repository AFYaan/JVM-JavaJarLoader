#include "Loader.h"
#include "LoaderUtils.h"
#include "Resources.h"

int main(int argsLength, const char* args[])
{
    //Resources file
    string resPath = GetExeDir() + string("\\JVM_TEST.jar");

    string mainMethod("pl/afyaan/Main");
    Loader* loader = new Loader(mainMethod);

    loader->RunFromMemory(rawData, sizeof(rawData), args, argsLength);
}