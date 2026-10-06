#include "filesystem.h"
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void seterr(char *e,size_t n,const char *fmt,const char *a){if(e&&n)snprintf(e,n,fmt,a?a:"");}
int dirsys_is_directory(const char *path){struct stat st; return path && stat(path,&st)==0 && S_ISDIR(st.st_mode);}
int dirsys_join_path(const char *base,const char *name,char *out,size_t n){
    if(!base||!name||!out||n==0||name[0]=='/'||strstr(name,"..")){return -1;}
    int rc=snprintf(out,n,"%s%s%s",base,(base[0]&&base[strlen(base)-1]=='/')?"":"/",name); return (rc<0||(size_t)rc>=n)?-1:0;
}
int dirsys_validate_blueprint(const char *root,char *error,size_t error_size){
    if(!root||!*root){seterr(error,error_size,"invalid blueprint root: %s","empty path");return -1;}
    struct stat st;if(lstat(root,&st)!=0){if(error&&error_size)snprintf(error,error_size,"blueprint '%s' cannot be accessed: %s",root,strerror(errno));return -1;}
    if(!S_ISDIR(st.st_mode)){seterr(error,error_size,"blueprint is not a directory: %s",root);return -1;}
    DIR *d=opendir(root);if(!d){if(error&&error_size)snprintf(error,error_size,"cannot open blueprint '%s': %s",root,strerror(errno));return -1;}
    struct dirent *ent;char path[4096];int rc=0;
    while((ent=readdir(d))){if(!strcmp(ent->d_name,".")||!strcmp(ent->d_name,".."))continue;
        if(dirsys_join_path(root,ent->d_name,path,sizeof(path))!=0){if(error&&error_size)snprintf(error,error_size,"invalid blueprint entry '%s'",ent->d_name);rc=-1;break;}
        if(lstat(path,&st)!=0){if(error&&error_size)snprintf(error,error_size,"cannot inspect blueprint entry '%s': %s",path,strerror(errno));rc=-1;break;}
        if(!S_ISDIR(st.st_mode)){if(error&&error_size)snprintf(error,error_size,"blueprint contains non-directory entry '%s'",path);rc=-1;break;}
        if(dirsys_validate_blueprint(path,error,error_size)!=0){rc=-1;break;}
    }
    closedir(d);return rc;
}
