#ifndef DIRSYS_GENERATOR_H
#define DIRSYS_GENERATOR_H
#include <stddef.h>
int dirsys_generate(const char *blueprint_root, const char *destination, char *error, size_t error_size);
#endif
