#include "parser.h"
int dirsys_parse_config(const char *path, DirsysConfig *config, char *error, size_t error_size) {
    return dirsys_config_load(path, config, error, error_size);
}
