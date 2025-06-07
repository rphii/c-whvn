#include "whvn-toplist-range.h"

Str whvn_toplist_range_str(WhvnToplistRangeList id) {
    switch(id) {
        case WHVN_TOPLIST_RANGE_NONE: return str("invalid");
        case WHVN_TOPLIST_RANGE_1D: return str("1D");
        case WHVN_TOPLIST_RANGE_3D: return str("3D");
        case WHVN_TOPLIST_RANGE_1W: return str("1W");
        case WHVN_TOPLIST_RANGE_1M: return str("1M");
        case WHVN_TOPLIST_RANGE_3M: return str("3M");
        case WHVN_TOPLIST_RANGE_6M: return str("6M");
        case WHVN_TOPLIST_RANGE_1Y: return str("1Y");
    }
}

