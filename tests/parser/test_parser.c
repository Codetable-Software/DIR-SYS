#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int run(const char*text){char path[]="/tmp/dirsys-parser-XXXXXX";int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"w");assert(f);fputs(text,f);fclose(f);DirsysConfig c;char e[256];int rc=dirsys_parse_config(path,&c,e,sizeof(e));unlink(path);return rc;}
int main(void){assert(run("GOOD=value\n") == 0);assert(run("BAD LINE\n") != 0);assert(run("BAD-KEY=value\n") != 0);assert(run("A=one\nA=two\n") != 0);assert(run("# only comment\n   # second\n") == 0);puts("test_parser: PASS");return 0;}
