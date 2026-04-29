#include <cstdio>

#ifdef _WIN32
FILE* _fopen(const char* path, const char* mode);
#else
#define _fopen fopen
#endif