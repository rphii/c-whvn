#ifndef WHVN_API_SEARCH_H

#include <rphii/str.h>
#include "whvn-category.h"
#include "whvn-purity.h"
#include "whvn-sorting.h"
#include "whvn-order.h"
#include "whvn-toplist-range.h"
#include "whvn-resolution.h"

typedef struct WhvnApiSearch {
    Str query;
    WhvnCategoryFlag categories;
    WhvnPurityFlag purity;
    WhvnSortingList sorting;
    WhvnOrderList order;
    WhvnToplistRangeList toplist_range;
    WhvnResolution atleast;
    WhvnResolutions resolutions;
    WhvnResolutions ratios;
    Color color;
    unsigned long page;
    Str seed;
} WhvnApiSearch;

#define WHVN_API_SEARCH_H
#endif
