#ifndef WHVN_META

#include <rlso.h>

typedef struct WhvnMeta {
    unsigned long current_page;
    unsigned long last_page;
    unsigned long per_page;
    unsigned long total;
    So query;
    So *seed;
} WhvnMeta;

#define WHVN_META
#endif

