#include <fstream>
#include <iostream>
#include <libgen.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 3 || strcmp(argv[1], "--file") != 0)
    {
        std::cout << "usage: --file <file name>" << std::endl;
        return EXIT_FAILURE;
    }
    std::ofstream stream(argv[2]);
    if (!stream.is_open())
    {
        std::cout << "Could not open " << argv[2] << " for writing" << std::endl;
        return EXIT_FAILURE;
    }
    stream << dirname(argv[0]);
    return EXIT_SUCCESS;
}
