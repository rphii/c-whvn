#include "whvn-api-search.h"

void whvn_api_search_fmt_websafe(Str *out, WhvnApiSearch *arg) {
    ASSERT_ARG(out);
    ASSERT_ARG(arg);
    size_t len = str_len_raw(*out);
    if(str_len_raw(arg->query)) {
        str_extend(out, str("q="));
        str_fmt_websafe(out, arg->query);
    }
    if(whvn_category_is_set(arg->categories)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_fmt(out, "categories=%u%u%u", 
                arg->categories.general,
                arg->categories.anime,
                arg->categories.people);
    }
    if(whvn_purity_is_set(arg->purity)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_fmt(out, "purity=%u%u%u", 
                arg->purity.sfw,
                arg->purity.sketchy,
                arg->purity.nsfw);
    }
    if(arg->sorting) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        switch(arg->sorting) {
            case WHVN_SORTING_DATE_ADDED: str_fmt(out, "sorting=date_added"); break;
            case WHVN_SORTING_RELEVANCE: str_fmt(out, "sorting=relevance"); break;
            case WHVN_SORTING_RANDOM: str_fmt(out, "sorting=random"); break;
            case WHVN_SORTING_VIEWS: str_fmt(out, "sorting=views"); break;
            case WHVN_SORTING_FAVORITES: str_fmt(out, "sorting=favorites"); break;
            case WHVN_SORTING_TOPLIST: str_fmt(out, "sorting=toplist"); break;
            default: break;
        }
    }
    if(arg->order) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        switch(arg->order) {
            case WHVN_ORDER_ASC: str_fmt(out, "order=asc"); break;
            case WHVN_ORDER_DESC: str_fmt(out, "order=desc"); break;
            default: break;
        }
    }
    if(arg->toplist_range) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        switch(arg->toplist_range) {
            case WHVN_TOPLIST_RANGE_1D: str_fmt(out, "topRange=1d"); break;
            case WHVN_TOPLIST_RANGE_3D: str_fmt(out, "topRange=3d"); break;
            case WHVN_TOPLIST_RANGE_1W: str_fmt(out, "topRange=1w"); break;
            case WHVN_TOPLIST_RANGE_1M: str_fmt(out, "topRange=1M"); break;
            case WHVN_TOPLIST_RANGE_3M: str_fmt(out, "topRange=3M"); break;
            case WHVN_TOPLIST_RANGE_6M: str_fmt(out, "topRange=6M"); break;
            case WHVN_TOPLIST_RANGE_1Y: str_fmt(out, "topRange=1y"); break;
            default: break;
        }
    }
    if(whvn_resolution_is_valid(arg->atleast)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_extend(out, str("atleast="));
        whvn_resolution_fmt(out, arg->atleast);
    }
    if(array_len(arg->ratios)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_extend(out, str("ratios="));
        whvn_ratios_fmt(out, arg->ratios);
    }
    if(array_len(arg->resolutions)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_extend(out, str("resolutions="));
        whvn_resolutions_fmt(out, arg->resolutions);
    }
    if(arg->page > 0) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_fmt(out, "page=%lu", arg->page);
    }
    if(str_len_raw(arg->seed)) {
        if(str_len_raw(*out) > len) str_push(out, '&');
        str_fmt(out, "seed=%.*s", STR_F(arg->seed));
    }
}

void whvn_api_search_free(WhvnApiSearch *arg) {
    ASSERT_ARG(arg);
    array_free(arg->resolutions);
    array_free(arg->ratios);
    memset(arg, 0, sizeof(*arg));
}

