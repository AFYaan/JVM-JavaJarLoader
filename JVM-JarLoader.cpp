#include "Loader.h"
#include "LoaderUtils.h"
#include "Resources.h"

int main(int argsLength, const char* args[])
{
    string mainMethod("pl/afyaan/Main");
    Loader* loader = new Loader(mainMethod.c_str());
    loader->RunFromMemory(rawData, sizeof(rawData));
}