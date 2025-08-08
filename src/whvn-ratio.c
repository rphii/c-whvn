#include "whvn-ratio.h"

void whvn_ratio_fmt(So *out, WhvnRatio ratio) {
    ASSERT_ARG(out);
    switch(ratio.type) {
        case WHVN_RATIO: {
            so_fmt(out, "%ux%u", ratio.w, ratio.h);
        } break;
        case WHVN_RATIO_LANDSCAPE: {
            so_extend(out, so("landscape"));
        } break;
        case WHVN_RATIO_PORTRAIT: {
            so_extend(out, so("portrait"));
        } break;
    }
}

void whvn_ratios_fmt(So *out, WhvnRatios ratios) {
    ASSERT_ARG(out);
    size_t len = array_len(ratios);
    for(size_t i = 0; i < len; ++i) {
        if(i) so_push(out, ',');
        whvn_ratio_fmt(out, array_at(ratios, i));
    }
}

WhvnRatio whvn_ratio_parse(So str) {
    So h, w = so_split_ch(str, 'x', &h);
    if(!h.len) {
        if(!so_cmp(str, so("landscape"))) return (WhvnRatio){ .type = WHVN_RATIO_LANDSCAPE };
        if(!so_cmp(str, so("portrait"))) return (WhvnRatio){ .type = WHVN_RATIO_PORTRAIT };
    }
    WhvnRatio result = {0};
    int err = 0;
    err |= so_as_int(h, (int *)&result.h, 10);
    err |= so_as_int(w, (int *)&result.w, 10);
    if(err) return (WhvnRatio){0};
    return result;
}

