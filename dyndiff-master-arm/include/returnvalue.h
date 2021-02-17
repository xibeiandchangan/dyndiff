#ifndef _RETURNVALUE_H_
#define _RETURNVALUE_H_
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
#include "forkserver.h"
#include "utils.h"

// function return value: 0 on success, 1 failed
// tested function argument should be passed in aArgv
// tested function return value will be place in *aRes
int returnvalue_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50], char *aRes);
#endif
