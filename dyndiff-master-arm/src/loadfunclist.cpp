#include <loadfunclist.h>

FuncinfoNodePtr loadFunctionList(const char* aFuncListBinName){
    FuncinfoNodePtr funclist;
    int fd;
    uint32_t funcsNum;

    funclist = NULL;
    if( ( fd = open(aFuncListBinName, O_RDONLY|O_CREAT) ) == -1 ){
        FATALERROR("open %s error\n", aFuncListBinName);
    }

    if( read(fd, &funcsNum, 4) != 4  ){
        FATALERROR("read number of function error\n");
    }
    LOGINFO("funcs num = %d\n", funcsNum);
    uint32_t i;
    FuncinfoNodePtr nodeptr;
    for(i = 0; i< funcsNum;i++){
        nodeptr = (FuncinfoNodePtr)malloc(sizeof(funcNode));
        read(fd, &(nodeptr->offset), OFFSETSIZE);
        read(fd, &(nodeptr->argNum), ARGNUMSIZE);
	read(fd, &(nodeptr->instruset), ARGNUMSIZE);
        nodeptr->next=funclist;
        funclist = nodeptr;
    }
    return funclist;
}

void printFuncinfo(FuncinfoNodePtr nodeptr){
    LOGINFO("%-10s%-8s%-8s","offset","argsLen","insSet");
    while(nodeptr){
        LOGINFO("0x%-8x%-8d%-8d",nodeptr->offset, nodeptr->argNum,nodeptr->instruset);
        nodeptr = nodeptr->next;
    }
}
