#include <loader.h>
#include <loadfunclist.h>
#include <forkserver.h>
#include <stdouterr.h>
#include <memcontent.h>

int main(){
    parseAndLoadSegsBin("./testdata/dumpsegs.bin");
    FuncinfoNodePtr allfuncs = loadFunctionList("./testdata/allfunc.bin");
    printFuncinfo(allfuncs);
    initforkserver();
  
    char argv[][50] = {"source","dest_string: dest_string","24"};   

    int testfd1;
    if((testfd1 = open("./testdata/testfile", O_RDWR|O_TRUNC)) == -1){
	FATALERROR("could not open ./testdata/testfile");
    }
    close(testfd1); 

    int testfd2;
    if((testfd2 = open("./testdata/std_file", O_RDWR|O_TRUNC)) == -1){
	FATALERROR("could not open ./testdata/std_file");
    }
    close(testfd2);

    while(allfuncs){
	char *res = NULL;
	if(allfuncs->argNum == 2){
	    LOGDEBUG("0x%-8x",allfuncs->offset);
	    if(allfuncs->instruset == 0){
			memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPP, argv, res);
			stdouterr_runtarget((void *)(allfuncs->offset+0x1001), IFPP, argv);
	    }else if(allfuncs->instruset == 1){
			memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPP, argv, res);
			stdouterr_runtarget((void *)(allfuncs->offset+0x1000), IFPP, argv);
	    }
	     
	}else if(allfuncs->argNum == 3){
	    LOGDEBUG("0x%-8x",allfuncs->offset);
	    if(allfuncs->instruset == 0){
	        memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPPI, argv, res);
	        stdouterr_runtarget((void *)(allfuncs->offset+0x1001), IFPP, argv);	     
	    }else if(allfuncs->instruset == 1){
	        memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPPI, argv, res);
	        stdouterr_runtarget((void *)(allfuncs->offset+0x1000), IFPP, argv);
	    }
	}else if(allfuncs->argNum == 4){
 	    LOGDEBUG("0x%-8x",allfuncs->offset);
	    if(allfuncs->instruset == 0){
	        memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPIPI, argv, res);
	        stdouterr_runtarget((void *)(allfuncs->offset+0x1001), IFPP, argv);	     
	    }else if(allfuncs->instruset == 1){
	        memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPIPI, argv, res);
	        stdouterr_runtarget((void *)(allfuncs->offset+0x1000), IFPP, argv);
	    }	   
	}
	allfuncs = allfuncs->next;
    }   
    LOGDEBUG("sleeping");
    sleep(100);
    return 0;
}
