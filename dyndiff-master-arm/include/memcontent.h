#ifndef _MEMCONTENT_H_
#define _MEMCONTENT_H_
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
int memcontent_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50]);
#endif
