#include <forkserver.h>

void initforkserver(){
    int serverpipe[2];
    if(pipe(serverpipe)){
        FATALERROR("create pipe error\n");
    }
    if( dup2(serverpipe[0], FORKSERVERREADFD) < 0 ){
        FATALERROR("dup2 serverpipe[0] error\n");
    }
    if( dup2(serverpipe[1], FORKSERVERWRITEFD) < 0 ){
        FATALERROR("dup2 serverpipe[1] error\n");
    }
    if( fcntl(FORKSERVERREADFD, F_SETFL, O_NONBLOCK) == -1 ){
        FATALERROR("fcntl error \n");
    }
    close(serverpipe[0]);
    close(serverpipe[1]);
}
void callfunc(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50]){
    if(aFuncType == IFPP){
	    func_ifpp_t tfuncptr;
        tfuncptr = (func_ifpp_t)aFuncPtr;
        LOGDEBUG("child processing");
	    tfuncptr(aArgv[0], aArgv[1]);
    }else if(aFuncType == IFPPI){
	    func_ifppi_t tfuncptr;
	    tfuncptr = (func_ifppi_t)aFuncPtr;
        LOGDEBUG("child processing");
	    tfuncptr(aArgv[0], aArgv[1], atoi(aArgv[2]));
    }else if(aFuncType == IFPIPI){
        func_ifpipi_t tfuncptr;
        tfuncptr = (func_ifpipi_t)aFuncPtr;
        LOGDEBUG("child processing");
        tfuncptr(aArgv[0], atoi(aArgv[2]), aArgv[1], atoi(aArgv[2]));
    }else{
        FATALERROR("unknown function prototype");
    }
}
void callfunc_memcontent(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50], char **aRes){
    if(aFuncType == IFPP){
	    func_ifpp_t tfuncptr;
        tfuncptr = (func_ifpp_t)aFuncPtr;
        LOGDEBUG("child processing");
        *aRes = (char *)tfuncptr(aArgv[0], aArgv[1]);
    }else if(aFuncType == IFPPI){
	    func_ifppi_t tfuncptr;
	    tfuncptr = (func_ifppi_t)aFuncPtr;
        LOGDEBUG("child processing");
	    *aRes = (char *)tfuncptr(aArgv[0], aArgv[1], atoi(aArgv[2]));
    }else{
        FATALERROR("unknown function prototype");
    }
}
void callfunc_mem_ret(void* aFuncPtr, funcprototype aFuncType, char aArgv[][50], uint32_t *re_val){
    if(aFuncType == IFPIPI){
        func_ifpipi_t tfuncptr;
        tfuncptr = (func_ifpipi_t)aFuncPtr;
        LOGDEBUG("child processing");
        *re_val = tfuncptr(aArgv[0], atoi(aArgv[2]), aArgv[1], atoi(aArgv[2]));
    }else{
        FATALERROR("unknown function prototype");
    }
}
