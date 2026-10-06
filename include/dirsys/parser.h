#ifndef DIRSYS_PARSER_H
#define DIRSYS_PARSER_H
#include <stddef.h>
#include "config.h"
int dirsys_parse_config(const char *path, DirsysConfig *config, char *error, size_t error_size);
#endif
