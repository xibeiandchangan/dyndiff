#ifndef _LOADER_H_
#define _LOADER_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "utils.h"


struct segstruct{
    char        name[0x20];
    uint32_t    segstart;
    uint32_t    segend;
    uint32_t    segperm;
    uint32_t    contsize;
};
struct seg_node{
    uint32_t    mmapstart;
    uint32_t    mmapend;
    struct seg_node* next;
};
void parseAndLoadSegsBin(const char* aSegBinName);
#endif
