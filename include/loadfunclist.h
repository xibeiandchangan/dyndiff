#ifndef _LOADFUNCLIST_H_
#define _LOADFUNCLIST_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "utils.h"

#define ARGNUMSIZE 2
#define OFFSETSIZE 4

typedef struct funcinfoNode{
    unsigned short argNum;
    uint32_t offset;
    unsigned short instruset;
    struct funcinfoNode * next;
}funcNode,*FuncinfoNodePtr;

FuncinfoNodePtr loadFunctionList(const char* aFuncListBinName);
void printFuncinfo(FuncinfoNodePtr nodeptr);
int printmempoint(FuncinfoNodePtr nodeptr);
#endif
