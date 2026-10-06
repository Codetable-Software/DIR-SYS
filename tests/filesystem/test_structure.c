#include "generator.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
static int d(const char*p){struct stat s;return stat(p,&s)==0&&S_ISDIR(s.st_mode);}
int main(void){char base[]="/tmp/dirsys-structure-XXXXXX";assert(mkdtemp(base));char out[512];snprintf(out,sizeof(out),"%s/out",base);char e[512];assert(dirsys_generate("directory/:/root",out,e,sizeof(e))==0);const char*paths[]={"bin","boot","dev","etc","etc/init","etc/network","etc/security","etc/system","home","home/usr","home/usr/Mobile","home/usr/Photo","home/usr/Video","home/usr/Audio","home/usr/Alarm","home/usr/Downloads","home/usr/Docs","home/usr/OS","home/usr/Apps","lib","lib32","lib64","lost+found","media","mnt","mnt/data","mnt/usb","mnt/removable","opt","proc","root","run","run/lock","run/mount","run/services","run/user","sbin","srv","srv/ftp","srv/http","srv/services","sys","sys/block","sys/bus","sys/class","sys/devices","sys/firmware","sys/fs","sys/kernel","sys/module","tmp","usr","usr/bin","usr/include","usr/lib","usr/lib32","usr/lib64","usr/local","usr/local/bin","usr/local/lib","usr/local/share","usr/sbin","usr/share","usr/share/applications","usr/share/docs","usr/share/icons","usr/share/locale","usr/share/man","var","var/cache","var/empty","var/lib","var/local","var/lock","var/log","var/mail","var/opt","var/run","var/spool","var/tmp"};char p[1024];for(size_t i=0;i<sizeof(paths)/sizeof(paths[0]);i++){snprintf(p,sizeof(p),"%s/%s",out,paths[i]);assert(d(p));}puts("test_structure: PASS");return 0;}
