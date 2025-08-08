#include "whvn-toplist-range.h"

So whvn_toplist_range_str(WhvnToplistRangeList id) {
    switch(id) {
        case WHVN_TOPLIST_RANGE_NONE: return so("invalid");
        case WHVN_TOPLIST_RANGE_1D: return so("1D");
        case WHVN_TOPLIST_RANGE_3D: return so("3D");
        case WHVN_TOPLIST_RANGE_1W: return so("1W");
        case WHVN_TOPLIST_RANGE_1M: return so("1M");
        case WHVN_TOPLIST_RANGE_3M: return so("3M");
        case WHVN_TOPLIST_RANGE_6M: return so("6M");
        case WHVN_TOPLIST_RANGE_1Y: return so("1Y");
    }
}

