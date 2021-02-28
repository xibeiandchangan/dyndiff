#ifndef _FORKSERVER_H_
#define _FORKSERVER_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include "utils.h"

#define FORKSERVERREADFD 0x123
#define FORKSERVERWRITEFD 0x124

// name convertion [return value type]F[argument value type]
// I: unsigned integer
// L: unsigned long long
// P: pointer(void*)
enum funcprototype{
    IF,
    IFI,
    IFII,
    IFIII,
    IFIIII,
    IFP,
    IFPP,
    IFPPI,
    IFPIPI
};
typedef uint32_t (*func_if_t)();
typedef uint32_t (*func_ifi_t)(uint32_t);
typedef uint32_t (*func_ifii_t)(uint32_t, uint32_t);
typedef uint32_t (*func_ifiii_t)(uint32_t, uint32_t, uint32_t);
typedef uint32_t (*func_ifiiii_t)(uint32_t, uint32_t, uint32_t, uint32_t);

typedef void (*func_ifp_t)(void *);
typedef void *(*func_ifpp_t)(void *, void *);
typedef void *(*func_ifppi_t)(void *, void *, uint32_t);
typedef void *(*func_ifpipi_t)(void *, uint32_t, void *, uint32_t);

void initforkserver();
void callfunc(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50]);
void callfunc_memcontent(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50], char **aRes);
void callfunc_mem_ret(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50], uint32_t *re_val);
#endif
