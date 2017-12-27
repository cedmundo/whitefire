#ifndef WFE_POOL_H
#define WFE_POOL_H
#include <wfe/types.h>

#define WFE_POOL_TINY   ((wfeSize) 512)         // 512 B
#define WFE_POOL_SMALL  ((wfeSize) 1024)        // 1 KB
#define WFE_POOL_MEDIUM ((wfeSize) 1048576)     // 1 MB
#define WFE_POOL_LARGE  ((wfeSize) 10485760)    // 10 MB
#define WFE_POOL_HUGE   ((wfeSize) 104857600)   // 100 MB

typedef struct wfeChunk {
    struct wfeChunk *next;
    wfeData *beg;
    wfeData *cur;
    wfeData *end;
} wfeChunk;

typedef struct wfePool {
    wfeChunk** tierChunks;
    wfeSize* tierSizes;
    wfeSize tierCount;
} wfePool;

#endif
