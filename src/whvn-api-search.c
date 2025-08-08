#include "whvn-api-search.h"

void whvn_api_search_fmt_websafe(So *out, WhvnApiSearch *arg) {
    ASSERT_ARG(out);
    ASSERT_ARG(arg);
    size_t len = so_len(*out);
    if(so_len(arg->query)) {
        so_extend(out, so("q="));
        so_fmt_websafe(out, arg->query);
    }
    if(whvn_category_is_set(arg->categories)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_fmt(out, "categories=%u%u%u", 
                arg->categories.general,
                arg->categories.anime,
                arg->categories.people);
    }
    if(whvn_purity_is_set(arg->purity)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_fmt(out, "purity=%u%u%u", 
                arg->purity.sfw,
                arg->purity.sketchy,
                arg->purity.nsfw);
    }
    if(arg->sorting) {
        if(so_len(*out) > len) so_push(out, '&');
        switch(arg->sorting) {
            case WHVN_SORTING_DATE_ADDED: so_fmt(out, "sorting=date_added"); break;
            case WHVN_SORTING_RELEVANCE: so_fmt(out, "sorting=relevance"); break;
            case WHVN_SORTING_RANDOM: so_fmt(out, "sorting=random"); break;
            case WHVN_SORTING_VIEWS: so_fmt(out, "sorting=views"); break;
            case WHVN_SORTING_FAVORITES: so_fmt(out, "sorting=favorites"); break;
            case WHVN_SORTING_TOPLIST: so_fmt(out, "sorting=toplist"); break;
            default: break;
        }
    }
    if(arg->order) {
        if(so_len(*out) > len) so_push(out, '&');
        switch(arg->order) {
            case WHVN_ORDER_ASC: so_fmt(out, "order=asc"); break;
            case WHVN_ORDER_DESC: so_fmt(out, "order=desc"); break;
            default: break;
        }
    }
    if(arg->toplist_range) {
        if(so_len(*out) > len) so_push(out, '&');
        switch(arg->toplist_range) {
            case WHVN_TOPLIST_RANGE_1D: so_fmt(out, "topRange=1d"); break;
            case WHVN_TOPLIST_RANGE_3D: so_fmt(out, "topRange=3d"); break;
            case WHVN_TOPLIST_RANGE_1W: so_fmt(out, "topRange=1w"); break;
            case WHVN_TOPLIST_RANGE_1M: so_fmt(out, "topRange=1M"); break;
            case WHVN_TOPLIST_RANGE_3M: so_fmt(out, "topRange=3M"); break;
            case WHVN_TOPLIST_RANGE_6M: so_fmt(out, "topRange=6M"); break;
            case WHVN_TOPLIST_RANGE_1Y: so_fmt(out, "topRange=1y"); break;
            default: break;
        }
    }
    if(whvn_resolution_is_valid(arg->atleast)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_extend(out, so("atleast="));
        whvn_resolution_fmt(out, arg->atleast);
    }
    if(array_len(arg->ratios)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_extend(out, so("ratios="));
        whvn_ratios_fmt(out, arg->ratios);
    }
    if(array_len(arg->resolutions)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_extend(out, so("resolutions="));
        whvn_resolutions_fmt(out, arg->resolutions);
    }
    if(arg->page > 0) {
        if(so_len(*out) > len) so_push(out, '&');
        so_fmt(out, "page=%lu", arg->page);
    }
    if(so_len(arg->seed)) {
        if(so_len(*out) > len) so_push(out, '&');
        so_fmt(out, "seed=%.*s", SO_F(arg->seed));
    }
}

void whvn_api_search_free(WhvnApiSearch *arg) {
    ASSERT_ARG(arg);
    array_free(arg->resolutions);
    array_free(arg->ratios);
    memset(arg, 0, sizeof(*arg));
}

