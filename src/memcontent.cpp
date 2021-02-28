#include <memcontent.h>

int memcontent_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50], char *aRes){
    LOGDEBUG("testing target memory content and return value");
    pid_t tpid = fork();
    if(tpid < 0){
        FATALERROR("fork error");
    }
    else if(tpid == 0){
        LOGDEBUG("in child processing");
        char msg[100];
	    uint32_t i;
	    uint32_t j;
        uint32_t re_value;

	    LOGDEBUG("Before running:");
        LOGDEBUG("argv[0]: %s",aArgv[0]);
        LOGDEBUG("argv[1]: %s",aArgv[1]);
	    memset(msg, '\0', sizeof(msg));

        if(aFuncType == IFPP){
	        LOGDEBUG("the number of arguments: 2");
            callfunc_memcontent(aFuncPtr, aFuncType, aArgv, &aRes);

        }else if(aFuncType == IFPPI){
	        LOGDEBUG("the number of arguments: 3")
	        callfunc_memcontent(aFuncPtr, aFuncType, aArgv, &aRes);
	    }else if(aFuncType == IFPIPI){
	        LOGDEBUG("the number of arguments: 4")
	        callfunc_mem_ret(aFuncPtr, aFuncType, aArgv, &re_value);
	    }
	    for(i = 0; i < strlen(aArgv[0]); i++){
	        msg[i] = aArgv[0][i];
	    }                        
	    for(j = 0; j < strlen(aArgv[1]); j++,i++){
	        msg[i] = aArgv[1][j];
	    }
	    LOGDEBUG("After running:");
        LOGDEBUG("argv[0]: %s",aArgv[0]);
        LOGDEBUG("argv[1]: %s",aArgv[1]);
	    if( strcmp(aArgv[0],"dest_string: dest_string") == 0 || strcmp(aArgv[0],"sourcedest_string: dest_string") == 0 ) {
	        if( strcmp(aRes,"dest_string: dest_string") == 0 || strcmp(aRes,"sourcedest_string: dest_string") == 0 || 
               == 0 ){
                int testfd1;
                if((testfd1 = open("./testdata/testfile", O_RDWR|O_APPEND)) == -1){
                    FATALERROR("could not open ./testdata/testfile");
                }
                if((write(testfd1, &(aFuncPtr), 4)) != 4){
                    FATALERROR("write testfile error");
                }
                if((write(testfd1, msg, 60)) != 60){
                    FATALERROR("write testfile error");
                }
                close(testfd1);
            }           
	    }
	    if(write(FORKSERVERWRITEFD,msg,100) != 100){
            FATALERROR("write to pipe error");
        }
        else{
            LOGDEBUG("write to pipe right");
        }
	    exit(0);        
    }
    else{
        char msg[100];
        int status;
        int readcount;
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
        	    }else if(WIFEXITED(status)){
            	    LOGDEBUG("exit status = 0x%x", WEXITSTATUS(status));
            	    readcount = read(FORKSERVERREADFD, msg , sizeof(msg));
                    if(readcount < 0){
                	    if(errno == EAGAIN){
                            LOGDEBUG("child process crashed");
                            return 1;
                	    }else{
                    	    FATALERROR("what");
                	    }
            	    }else if(readcount == 0){
                	    FATALERROR("pipe end");
            	    }else{
                	    LOGDEBUG("After running:");
                	    if(aFuncType == IFPP){
                    	    LOGDEBUG("argv: %s",msg);
                    	    return 0;
                	    }else if(aFuncType == IFPPI){
                    	    LOGDEBUG("argv: %s",msg);
                    	    return 0;
                	    }else if(aFuncType == IFPIPI){
                    	    LOGDEBUG("argv: %s",msg);
                    	    return 0;
                	    }
            	    }
                }
	        }
	    }       
	    return 1;
    }
}
