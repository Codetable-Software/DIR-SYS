#include "generator.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
static int d(const char*p){struct stat s;return stat(p,&s)==0&&S_ISDIR(s.st_mode);} static void m(const char*p){assert(mkdir(p,0755)==0);}
int main(void){char base[]="/tmp/dirsys-map-XXXXXX";assert(mkdtemp(base));char src[512],out[512],h[512];snprintf(src,sizeof(src),"%s/root",base);snprintf(out,sizeof(out),"%s/output",base);m(src);snprintf(h,sizeof(h),"%s/home",src);m(h);snprintf(h,sizeof(h),"%s/home/usr",src);m(h);snprintf(h,sizeof(h),"%s/home/usr/Photo",src);m(h);snprintf(h,sizeof(h),"%s/etc",src);m(h);char e[256];assert(dirsys_generate(src,out,e,sizeof(e))==0);char p[512];snprintf(p,sizeof(p),"%s/home/usr/Photo",out);assert(d(p));snprintf(p,sizeof(p),"%s/etc",out);assert(d(p));snprintf(p,sizeof(p),"%s/root",out);assert(!d(p));puts("test_mapping: PASS");return 0;}
