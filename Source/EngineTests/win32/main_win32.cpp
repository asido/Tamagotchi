#include <string>
#include <iostream>
#include "TestMain.h"

int main(int argc, char *argv[])
{
    const std::string result = TestMain();
    std::cout << result;

    (void)getchar();

    return 0;
}