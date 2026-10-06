#include "generator.h"
#include "filesystem.h"
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int mkdir_if_needed(const char *path,char *error,size_t n){
    struct stat st;
    if(lstat(path,&st)==0){if(!S_ISDIR(st.st_mode)){if(error&&n)snprintf(error,n,"destination exists but is not a directory: '%s'",path);return -1;}return 0;}
    if(errno!=ENOENT){if(error&&n)snprintf(error,n,"cannot inspect destination '%s': %s",path,strerror(errno));return -1;}
    if(mkdir(path,0755)!=0 && errno!=EEXIST){if(error&&n)snprintf(error,n,"cannot create directory '%s': %s",path,strerror(errno));return -1;}
    return 0;
}
static int copy_tree(const char *src,const char *dst,char *error,size_t n){
    if(mkdir_if_needed(dst,error,n)!=0)return -1;
    DIR *d=opendir(src);if(!d){if(error&&n)snprintf(error,n,"cannot open blueprint '%s': %s",src,strerror(errno));return -1;}
    struct dirent *e;char s[4096],t[4096];int rc=0;
    while((e=readdir(d))){if(!strcmp(e->d_name,".")||!strcmp(e->d_name,".."))continue;
        if(dirsys_join_path(src,e->d_name,s,sizeof(s))||dirsys_join_path(dst,e->d_name,t,sizeof(t))){if(error&&n)snprintf(error,n,"path too long while generating '%s'",e->d_name);rc=-1;break;}
        struct stat st;if(lstat(s,&st)!=0){if(error&&n)snprintf(error,n,"cannot inspect '%s': %s",s,strerror(errno));rc=-1;break;}
        if(!S_ISDIR(st.st_mode)){if(error&&n)snprintf(error,n,"blueprint contains non-directory entry '%s'",s);rc=-1;break;}
        if(copy_tree(s,t,error,n)!=0){rc=-1;break;}
    }
    closedir(d);return rc;
}
int dirsys_generate(const char *blueprint_root,const char *destination,char *error,size_t error_size){
    if(!blueprint_root||!destination||!*blueprint_root||!*destination){if(error&&error_size)snprintf(error,error_size,"blueprint and destination are required");return -1;}
    if(dirsys_validate_blueprint(blueprint_root,error,error_size)!=0)return -1;
    if(mkdir_if_needed(destination,error,error_size)!=0)return -1;
    return copy_tree(blueprint_root,destination,error,error_size);
}
