#ifndef WHVN_API_SEARCH_H

#include <rphii/str.h>
#include "whvn-category.h"
#include "whvn-purity.h"
#include "whvn-sorting.h"
#include "whvn-order.h"
#include "whvn-toplist-range.h"
#include "whvn-resolution.h"
#include "whvn-ratio.h"

typedef struct WhvnApiSearch {
    Str                     query;
    WhvnCategory            categories;
    WhvnPurity              purity;
    WhvnSortingList         sorting;
    WhvnOrderList           order;
    WhvnToplistRangeList    toplist_range;
    WhvnResolution          atleast;
    WhvnResolutions         resolutions;
    WhvnRatios              ratios;
    Color                   color;
    unsigned long           page;
    Str                     seed;
} WhvnApiSearch;

void whvn_api_search_fmt_websafe(Str *out, WhvnApiSearch *arg);
void whvn_api_search_free(WhvnApiSearch *arg);

#define WHVN_API_SEARCH_H
#endif
