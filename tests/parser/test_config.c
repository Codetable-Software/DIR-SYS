#include "config.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void){char path[]="/tmp/dirsys-config-XXXXXX";int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");assert(f);fprintf(f,"# comment\n OS_NAME = TestOS \nOS_VERSION=1.2.3 # inline\nKERNEL_PARAMETERS=\"quiet loglevel=3\"\n");fclose(f);DirsysConfig c;char e[256];assert(dirsys_config_load(path,&c,e,sizeof(e))==0);assert(c.count==3);assert(!strcmp(dirsys_config_get(&c,"OS_NAME"),"TestOS"));assert(!strcmp(dirsys_config_get(&c,"OS_VERSION"),"1.2.3"));assert(!strcmp(dirsys_config_get(&c,"KERNEL_PARAMETERS"),"quiet loglevel=3"));unlink(path);puts("test_config: PASS");return 0;}
