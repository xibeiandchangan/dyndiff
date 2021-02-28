#include <loader.h>

void parseAndLoadSegsBin(const char* aSegBinName){
    struct seg_node* seg_root;
    int segsbinfd;
    uint32_t segcount;
    uint32_t i;

    seg_root = NULL;
    if( (segsbinfd = open(aSegBinName, O_RDONLY)) == -1 ){
        FATALERROR("could not open %s", aSegBinName);
    }
    if( read(segsbinfd, &segcount, 4) != 4 ){
        FATALERROR("read seg count error");
    }
    LOGINFO("segment count = 0x%x", segcount);
    LOGINFO("%-20s %-18s %-18s %-10s %-10s",
            "name",
            "start",
            "end",
            "perm",
            "size");
    for(i=0; i<segcount; i++){
        struct segstruct tseg;
        uint8_t *contptr;
        if( read(segsbinfd, &tseg, sizeof(struct segstruct)) != sizeof(struct segstruct) ){
            FATALERROR("read segstruct error");
        }
        LOGINFO("%-20s 0x%-16x 0x%-16x 0x%-8x 0x%-8x",
                tseg.name,
                tseg.segstart,
                tseg.segend,
                tseg.segperm,
                tseg.contsize);

        contptr = (uint8_t*)malloc(tseg.contsize);
        if( contptr == NULL ){
            FATALERROR("malloc error");
        }
        if( read(segsbinfd, contptr, tseg.contsize) != tseg.contsize ){
            FATALERROR("read content error");
        }
        struct seg_node* node_ptr = seg_root;
        uint32_t mmapstart = 0;
        uint32_t mmapend = 0;
        while(node_ptr){
            if( tseg.segstart < node_ptr->mmapstart && node_ptr->mmapend <= tseg.segend ){
                //| [ ] |
                break;
            }
            if( node_ptr->mmapstart <= tseg.segstart && tseg.segend <= node_ptr->mmapend ){
                //[ | | ]
                break;
            }
            if( node_ptr->mmapstart <= tseg.segstart && tseg.segstart < node_ptr->mmapend &&
                    node_ptr->mmapend < tseg.segend ){
                //[ | ] |
                mmapstart = node_ptr->mmapend;
                if(tseg.segend & 0xfff){
                    mmapend = ((tseg.segend>>12) + 1)<<12;
                }else{
                    mmapend = tseg.segend;
                }
                break;
            }
            if( node_ptr->mmapstart <= tseg.segend && tseg.segend < node_ptr->mmapend &&
                    tseg.segstart < node_ptr->mmapstart ){
                //| [ | ]
                mmapstart = (tseg.segstart>>12)<<12;
                mmapend = node_ptr->mmapstart;
                break;
            }
            node_ptr = node_ptr->next;
        }
        if( node_ptr == NULL ){
            // |  | [  ]
            mmapstart = (tseg.segstart>>12)<<12;
            if(tseg.segend & 0xfff){
                mmapend = ((tseg.segend>>12) + 1)<<12;
            }else{
                mmapend = tseg.segend;
            }
        }
        if( mmapstart != 0 ){
            struct seg_node* newnode = (struct seg_node*)malloc(sizeof(struct seg_node));
            memset(newnode, 0, sizeof(*newnode));
            newnode->mmapstart = mmapstart;
            newnode->mmapend = mmapend;
            newnode->next = seg_root;
            seg_root = newnode;
            if( mmap(
                        (void*)newnode->mmapstart,
                        newnode->mmapend - newnode->mmapstart,
                        7,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1,
                        0
                    ) != ((void*)(newnode->mmapstart)) ){
                LOGDEBUG("%p 0x%x", (void*)newnode->mmapstart,
                        newnode->mmapend - newnode->mmapstart);
                FATALERROR("mmap error");
            }

        }
        memcpy((void*)tseg.segstart, contptr, tseg.contsize);
        free(contptr);
    }
    struct seg_node* node_ptr = seg_root;
    struct seg_node* lastnode;
    while(node_ptr){
        lastnode = node_ptr;
        node_ptr = node_ptr->next;
        free(lastnode);
    }
    close(segsbinfd);
}
