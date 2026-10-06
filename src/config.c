#include "config.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

static void set_error(char *error, size_t n, const char *msg) {
    if (error && n) snprintf(error, n, "%s", msg);
}
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) ++s;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    return s;
}
static int valid_key(const char *key) {
    if (!key || !*key) return 0;
    for (const unsigned char *p=(const unsigned char*)key; *p; ++p)
        if (!(isalnum(*p) || *p=='_')) return 0;
    return 1;
}
static int unquote(char *value) {
    size_t len = strlen(value);
    if (len >= 2 && value[0]=='"' && value[len-1]=='"') {
        value[len-1]='\0';
        memmove(value, value+1, len-1);
        return 1;
    }
    if ((value[0]=='"') != (len && value[len-1]=='"')) return 0;
    return 1;
}
void dirsys_config_init(DirsysConfig *config) { if (config) memset(config, 0, sizeof(*config)); }
const char *dirsys_config_get(const DirsysConfig *config, const char *key) {
    if (!config || !key) return NULL;
    for (size_t i=0;i<config->count;i++) if (strcmp(config->entries[i].key,key)==0) return config->entries[i].value;
    return NULL;
}
int dirsys_config_set(DirsysConfig *config, const char *key, const char *value, char *error, size_t error_size) {
    if (!config || !valid_key(key) || !value || !*value) { set_error(error,error_size,"invalid key or empty value"); return -1; }
    if (strlen(key)>=DIRSYS_CONFIG_KEY_MAX || strlen(value)>=DIRSYS_CONFIG_VALUE_MAX) { set_error(error,error_size,"key or value is too long"); return -1; }
    for (size_t i=0;i<config->count;i++) if (strcmp(config->entries[i].key,key)==0) { set_error(error,error_size,"duplicate configuration key"); return -1; }
    if (config->count >= DIRSYS_CONFIG_MAX_ENTRIES) { set_error(error,error_size,"too many configuration entries"); return -1; }
    snprintf(config->entries[config->count].key, DIRSYS_CONFIG_KEY_MAX, "%s", key);
    snprintf(config->entries[config->count].value, DIRSYS_CONFIG_VALUE_MAX, "%s", value);
    config->count++;
    return 0;
}
int dirsys_config_load(const char *path, DirsysConfig *config, char *error, size_t error_size) {
    if (!path || !config) { set_error(error,error_size,"configuration path or object is null"); return -1; }
    FILE *fp=fopen(path,"r");
    if (!fp) { if(error&&error_size) snprintf(error,error_size,"cannot open '%s': %s",path,strerror(errno)); return -1; }
    dirsys_config_init(config);
    char line[2048]; unsigned long lineno=0;
    while (fgets(line,sizeof(line),fp)) {
        lineno++;
        if (!strchr(line,'\n') && !feof(fp)) { fclose(fp); if(error&&error_size) snprintf(error,error_size,"line %lu is too long",lineno); return -1; }
        char *s=trim(line); if (!*s || *s=='#') continue;
        char *comment=NULL; int quoted=0;
        for (char *p=s;*p;p++) { if(*p=='"') quoted=!quoted; else if(*p=='#'&&!quoted){comment=p;break;} }
        if(comment){*comment='\0';s=trim(s);if(!*s)continue;}
        char *eq=strchr(s,'=');
        if(!eq){fclose(fp);if(error&&error_size)snprintf(error,error_size,"line %lu: expected KEY=VALUE",lineno);return -1;}
        *eq='\0'; char *key=trim(s); char *value=trim(eq+1);
        if(!valid_key(key)){fclose(fp);if(error&&error_size)snprintf(error,error_size,"line %lu: invalid key '%s'",lineno,key);return -1;}
        if(!*value || !unquote(value)){fclose(fp);if(error&&error_size)snprintf(error,error_size,"line %lu: invalid value",lineno);return -1;}
        if(dirsys_config_set(config,key,value,error,error_size)!=0){char prior[1024];snprintf(prior,sizeof(prior),"line %lu: %s",lineno,error?error:"");if(error&&error_size)snprintf(error,error_size,"%s",prior);fclose(fp);return -1;}
    }
    if(ferror(fp)){fclose(fp);set_error(error,error_size,"error while reading configuration");return -1;}
    fclose(fp); return 0;
}
