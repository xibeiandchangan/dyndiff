#include <stdouterr.h>


int stdouterr_runtarget(void *aFuncPtr, funcprototype aFuncType, char aArgv[][50]){
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

        if(aFuncType == IFPP){
            LOGDEBUG("the number of arguments: 2");
            callfunc(aFuncPtr, aFuncType, aArgv);
        }else if(aFuncType == IFPPI){
            LOGDEBUG("the number of arguments: 3")
            callfunc(aFuncPtr, aFuncType, aArgv);
        }else if(aFuncType == IFPIPI){
            LOGDEBUG("the number of arguments: 4")
            callfunc(aFuncPtr, aFuncType, aArgv);
        }
        exit(0);
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
            int testfd2;
            if((testfd2 = open("./testdata/std_file", O_RDWR|O_APPEND)) == -1){
                FATALERROR("could not open ./testdata/std_file");
            }
            while (read(stdout_pipe[0], buf, 1024) > 0){
                LOGDEBUG("%s%s%s","stdout:`",buf,"`");
                if((write(testfd2, buf, 1024)) != 1024){
                    FATALERROR("write testfile error");
                }
                memset(buf,0,1025);
            }
            LOGDEBUG("reading stdout end.");
            LOGDEBUG("reading stderr..");
            while (read(stderr_pipe[0], buf, 1024) > 0){
                LOGDEBUG("%s%s%s","stderr:`",buf,"`");
                if((write(testfd2, buf, 1024)) != 1024){
                    FATALERROR("write testfile error");
                }
                memset(buf,0,1025);
            }
            close(testfd2);
            LOGDEBUG("reading stderr end.");
            return 0;
        }
    }
}
