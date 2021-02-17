#include <stdouterr.h>


int stdouterr_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50], char *aRes){
    LOGDEBUG("testing target stdout & stderr");

    int stdout_pipe[2];
    int stderr_pipe[2];

    if(pipe(stderr_pipe)){
        FATALERROR("create stderr pipe error\n");
    }
    if(pipe(stdout_pipe)){
        FATALERROR("create stdout pipe error\n");
    }
    pid_t tpid;
    tpid = fork();
    if(tpid < 0){
        FATALERROR("fork error");
    } else if(tpid == 0){
        // child
        LOGDEBUG("in stdouterr child process");
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        // call func
        //uint64_t tres;
        //callfunc(aFuncPtr, aFuncType, aArgv, &tres);
	callfunc(aFuncPtr, aFuncType, aArgv, &aRes);

        // asure crash child process to notify parent
        // if child crash here, message is sent, so parent will know function invocation is successful.
        int* ptr = (int*)0x41414141;
        *ptr = 1;
        exit(-1);
    }
    else {
        // parent
        LOGDEBUG("in stdouterr parent process");

        uint8_t buf[1025];
        memset(buf,0,1025);
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
        
        fd_set set;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500;
        FD_ZERO(&set);
        FD_SET(stdout_pipe[0], &set);
        FD_SET(stderr_pipe[0], &set);
        int hstdoutfd = stdout_pipe[0] > stderr_pipe[0] ? stdout_pipe[0] + 1 : stderr_pipe[0] + 1;
        int rv = select(hstdoutfd, &set, NULL, NULL, &timeout);
        if(rv == -1){
            FATALERROR("select() error.");
            return 1;
        } else if(rv == 0){
            LOGDEBUG("timeout. kill ...");
            kill(tpid, SIGKILL);
            return 1;
        } else{
            LOGDEBUG("reading stdout..");
            while (read(stdout_pipe[0], buf, 1024) > 0){
                LOGDEBUG("%s%s%s","stdout:`",buf,"`");
                memset(buf,0,1025);
            }
            LOGDEBUG("reading stdout end.");
            LOGDEBUG("reading stderr..");
            while (read(stderr_pipe[0], buf, 1024) > 0){
                LOGDEBUG("%s%s%s","stderr:`",buf,"`");
                memset(buf,0,1025);
            }
            LOGDEBUG("reading stderr end.");
            return 0;
        }
    }
}
