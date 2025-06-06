#ifndef WHVN_META

#include <rphii/str.h>

typedef struct WhvnMeta {
    unsigned long current_page;
    unsigned long last_page;
    unsigned long per_page;
    unsigned long total;
    Str query;
    Str *seed;
} WhvnMeta;

#define WHVN_META
#endif

