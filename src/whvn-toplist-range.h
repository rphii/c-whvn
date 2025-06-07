#ifndef WHVN_TOPLIST_RANGE

typedef enum {
    WHVN_TOPLIST_RANGE_NONE,
    /* keep below */
    WHVN_TOPLIST_RANGE_1D,
    WHVN_TOPLIST_RANGE_3D,
    WHVN_TOPLIST_RANGE_1W,
    WHVN_TOPLIST_RANGE_1M,
    WHVN_TOPLIST_RANGE_3M,
    WHVN_TOPLIST_RANGE_6M,
    WHVN_TOPLIST_RANGE_1Y,
} WhvnToplistRangeList;

#include <rphii/str.h>

Str whvn_toplist_range_str(WhvnToplistRangeList id);

#define WHVN_TOPLIST_RANGE
#endif
