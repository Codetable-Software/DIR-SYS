#ifndef DIRSYS_FILESYSTEM_H
#define DIRSYS_FILESYSTEM_H
#include <stddef.h>
int dirsys_validate_blueprint(const char *root, char *error, size_t error_size);
int dirsys_is_directory(const char *path);
int dirsys_join_path(const char *base, const char *name, char *out, size_t out_size);
#endif
