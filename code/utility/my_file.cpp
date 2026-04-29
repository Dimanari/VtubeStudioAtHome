#include "my_file.hpp"

#ifdef _WIN32
FILE* _fopen(const char* path, const char* mode)
{
    FILE* file;
    int result = fopen_s(&file, path, mode);
    (void)result; //Unused
    return file;
}
#endif
