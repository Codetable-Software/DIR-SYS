#include "dirsys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

static void usage(const char *p){fprintf(stderr,"Usage: %s generate [--config PATH] [--output PATH]\n       %s validate [--config PATH]\n",p,p);}
int main(int argc,char **argv){
    if(argc<2){usage(argv[0]);return 2;}
    const char *cmd=argv[1],*config_path="directory/:/dirsys.conf",*output_override=NULL;
    for(int i=2;i<argc;i++){if(!strcmp(argv[i],"--config")&&i+1<argc)config_path=argv[++i];else if(!strcmp(argv[i],"--output")&&i+1<argc)output_override=argv[++i];else{usage(argv[0]);return 2;}}
    DirsysConfig cfg;char err[1024];
    if(dirsys_parse_config(config_path,&cfg,err,sizeof(err))!=0){fprintf(stderr,"DIR-SYS: config error: %s\n",err);return 1;}
    const char *root_template=dirsys_config_get(&cfg,"ROOT_TEMPLATE");
    const char *configured_output=dirsys_config_get(&cfg,"OUTPUT_PATH");
    if(!root_template||!configured_output){fprintf(stderr,"DIR-SYS: config must define ROOT_TEMPLATE and OUTPUT_PATH\n");return 1;}
    char blueprint[PATH_MAX];snprintf(blueprint,sizeof(blueprint),"directory/:/%s",root_template);
    const char *destination=output_override?output_override:configured_output;
    if(!strcmp(cmd,"validate")){if(dirsys_validate_blueprint(blueprint,err,sizeof(err))!=0){fprintf(stderr,"DIR-SYS: invalid blueprint: %s\n",err);return 1;}printf("DIR-SYS: blueprint is valid: %s\n",blueprint);return 0;}
    if(!strcmp(cmd,"generate")){if(dirsys_generate(blueprint,destination,err,sizeof(err))!=0){fprintf(stderr,"DIR-SYS: generation failed: %s\n",err);return 1;}printf("DIR-SYS: generated %s -> %s\n",blueprint,destination);return 0;}
    usage(argv[0]);return 2;
}
