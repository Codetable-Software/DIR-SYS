#include "generator.h"
#include "filesystem.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
static void mk(const char*p){assert(mkdir(p,0755)==0);} static int isdir(const char*p){struct stat s;return stat(p,&s)==0&&S_ISDIR(s.st_mode);} 
int main(void){char base[]="/tmp/dirsys-gen-XXXXXX";assert(mkdtemp(base));char src[4096],dst[4096];snprintf(src,sizeof(src),"%s/src",base);snprintf(dst,sizeof(dst),"%s/out",base);mk(src);char a[4096];snprintf(a,sizeof(a),"%s/a",src);mk(a);snprintf(a,sizeof(a),"%s/a/b",src);mk(a);snprintf(a,sizeof(a),"%s/a/b/c",src);mk(a);char e[256];assert(dirsys_generate(src,dst,e,sizeof(e))==0);assert(isdir("/tmp"));char p[4096];snprintf(p,sizeof(p),"%s/a/b/c",dst);assert(isdir(p));assert(dirsys_generate(src,dst,e,sizeof(e))==0);FILE*f=fopen(dst,"w");assert(!f);assert(dirsys_generate("/definitely/not/a/blueprint",dst,e,sizeof(e))!=0);char bad[4096];snprintf(bad,sizeof(bad),"%s/bad",base);FILE*x=fopen(bad,"w");assert(x);fputs("file",x);fclose(x);assert(dirsys_generate(bad,dst,e,sizeof(e))!=0);char bad_destination[4096];snprintf(bad_destination,sizeof(bad_destination),"%s/destination-file",base);FILE*y=fopen(bad_destination,"w");assert(y);fputs("not a directory",y);fclose(y);assert(dirsys_generate(src,bad_destination,e,sizeof(e))!=0);char joined[128];assert(dirsys_join_path(src,"../escape",joined,sizeof(joined))!=0);assert(dirsys_join_path(src,"/absolute",joined,sizeof(joined))!=0);puts("test_generator: PASS");return 0;}
