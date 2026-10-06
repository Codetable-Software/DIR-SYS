#ifndef DIRSYS_CONFIG_H
#define DIRSYS_CONFIG_H

#include <stddef.h>

#define DIRSYS_CONFIG_KEY_MAX 64
#define DIRSYS_CONFIG_VALUE_MAX 1024
#define DIRSYS_CONFIG_MAX_ENTRIES 128

typedef struct {
    char key[DIRSYS_CONFIG_KEY_MAX];
    char value[DIRSYS_CONFIG_VALUE_MAX];
} DirsysConfigEntry;

typedef struct {
    DirsysConfigEntry entries[DIRSYS_CONFIG_MAX_ENTRIES];
    size_t count;
} DirsysConfig;

void dirsys_config_init(DirsysConfig *config);
int dirsys_config_load(const char *path, DirsysConfig *config, char *error, size_t error_size);
const char *dirsys_config_get(const DirsysConfig *config, const char *key);
int dirsys_config_set(DirsysConfig *config, const char *key, const char *value, char *error, size_t error_size);

#endif
