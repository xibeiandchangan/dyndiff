#include <loader.h>
#include <loadfunclist.h>
#include <forkserver.h>
#include <returnvalue.h>
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

    while(allfuncs){
	//char *res = NULL;
	if(allfuncs->argNum == 2){
	     LOGDEBUG("0x%-8x",allfuncs->offset);
	     if(allfuncs->instruset == 0){
		//returnvalue_runtarget((void *)(allfuncs->offset+1), IFPP, argv, res);
		memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPP, argv);	     
	     }else if(allfuncs->instruset == 1){
		//returnvalue_runtarget((void *)(allfuncs->offset), IFPP, argv, res);
		memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPP, argv);
	     }
	     
	}else if(allfuncs->argNum == 3){
	     LOGDEBUG("0x%-8x",allfuncs->offset);
	     if(allfuncs->instruset == 0){
		//returnvalue_runtarget((void *)(allfuncs->offset+1), IFPPI, argv, res);
	        memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPPI, argv);	     
	     }else if(allfuncs->instruset == 1){
		//returnvalue_runtarget((void *)(allfuncs->offset), IFPPI, argv, res);
	        memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPPI, argv);
	     }
	}else if(allfuncs->argNum == 4){
 	     LOGDEBUG("0x%-8x",allfuncs->offset);
	     if(allfuncs->instruset == 0){
		//returnvalue_runtarget((void *)(allfuncs->offset+1), IFPPI, argv, res);
	        memcontent_runtarget((void *)(allfuncs->offset+0x1001), IFPIPI, argv);	     
	     }else if(allfuncs->instruset == 1){
		//returnvalue_runtarget((void *)(allfuncs->offset), IFPPI, argv, res);
	        memcontent_runtarget((void *)(allfuncs->offset+0x1000), IFPIPI, argv);
	     }	   
	}
	allfuncs = allfuncs->next;
    }
    
    LOGDEBUG("sleeping");
    sleep(100);
    return 0;
}
