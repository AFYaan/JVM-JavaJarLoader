#include <iostream>
#include <windows.h>

#include "LoaderUtils.h"

#include "Loader.h"


using namespace std;

int main(int argsLength, const char* args[])
{
    Loader* loader = new Loader();
    loader->Run(args, argsLength);
}