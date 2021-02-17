#include <stdint.h>
#include <returnvalue.h>

int returnvalue_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50], char *aRes){
    LOGDEBUG("testing target return value");
    pid_t tpid = fork();
    if(tpid < 0){
        FATALERROR("fork error");
    }else if(tpid == 0){
        LOGDEBUG("in child process");
        //uint8_t msg[0x100];
        //uint32_t *intmsgptr = (uint32_t*)msg;
        //uint64_t tres;

        // call function
        //callfunc(aFuncPtr, aFuncType, aArgv, &tres);
	
	if(aFuncType == IFPP){

	     LOGDEBUG("the number of arguments: 2")

             callfunc(aFuncPtr, aFuncType, aArgv, &aRes);
	}else if(aFuncType == IFPPI){

	     LOGDEBUG("the number of arguments: 3")

             callfunc(aFuncPtr, aFuncType, aArgv, &aRes);
	}
        
	LOGDEBUG("return_value: %s",aRes);
        exit(0);
    }else{
        //char msg[0x100];
        //uint32_t *intmsgptr = (uint32_t*)msg;
        //ssize_t readcount;
        int status;
	int count = 0;

        LOGDEBUG("parent");
	while(1){
	    sleep(1);
	    pid_t result = waitpid(tpid, &status, WNOHANG);	    
	    
	    if(result == 0){//process is running
		count++;
	   	if(count >= 10){
		     kill(tpid, 9);
		     LOGDEBUG("The child process has been killed");
		     return 1;
		}
	    }else if(result == tpid){
		if(WIFSIGNALED(status)){
            	     LOGDEBUG("child get signal %d",WTERMSIG(status));
                     return 1;
        	}
        	else if(WIFEXITED(status)){
            	     LOGDEBUG("exit status = 0x%x", WEXITSTATUS(status));
		     return 1;
                }
	    }
	}
        /*wait(&status);
        if(WIFSIGNALED(status)){
            LOGDEBUG("child get signal %d",WTERMSIG(status));
	    return 1;
            readcount = read(FORKSERVERREADFD, msg, sizeof(msg));
            if( readcount < 0 ){
                if (errno == EAGAIN) {
                    LOGDEBUG("child process crashed");
                    return 1;
                }else{
                    FATALERROR("what");
                }
            }else if( readcount == 0 ){
                FATALERROR("pipe end");
            }else{
                LOGDEBUG("result = 0x%x", intmsgptr[1]);
                aRes[0] = intmsgptr[1];
                return 0;
            }
        }else if(WIFEXITED(status)){
            LOGDEBUG("child exit status = 0x%x", WEXITSTATUS(status));
            return 1;
        }*/
        return 1;
    }
}
