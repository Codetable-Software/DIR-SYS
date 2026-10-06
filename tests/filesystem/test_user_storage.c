#include "generator.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
static int d(const char*p){struct stat s;return stat(p,&s)==0&&S_ISDIR(s.st_mode);}
int main(void){char base[]="/tmp/dirsys-user-XXXXXX";assert(mkdtemp(base));char out[512],p[512];snprintf(out,sizeof(out),"%s/out",base);char e[256];assert(dirsys_generate("directory/:/root",out,e,sizeof(e))==0);const char*names[]={"Mobile","Photo","Video","Audio","Alarm","Downloads","Docs","OS","Apps"};for(size_t i=0;i<sizeof(names)/sizeof(names[0]);i++){snprintf(p,sizeof(p),"%s/home/usr/%s",out,names[i]);assert(d(p));}snprintf(p,sizeof(p),"%s/home/usr",out);assert(d(p));puts("test_user_storage: PASS");return 0;}
