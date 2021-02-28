#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOGINFO(aFmt...) \
    printf("\033[32m[+] "); \
    printf(aFmt); \
    printf("\033[0m\n");

#define LOGDEBUG(aFmt...) \
    printf("\033[1;34m[*] "); \
    printf(aFmt); \
    printf("\033[0m\n");

#define FATALERROR(aFmt...) \
    printf("\033[31m[-] "); \
    printf(aFmt); \
    printf("\033[0m\n"); \
    exit(-1)
