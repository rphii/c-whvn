#include "whvn-ratio.h"

void whvn_ratio_fmt(Str *out, WhvnRatio ratio) {
    ASSERT_ARG(out);
    switch(ratio.type) {
        case WHVN_RATIO: {
            str_fmt(out, "%ux%u", ratio.w, ratio.h);
        } break;
        case WHVN_RATIO_LANDSCAPE: {
            str_extend(out, str("landscape"));
        } break;
        case WHVN_RATIO_PORTRAIT: {
            str_extend(out, str("portrait"));
        } break;
    }
}

void whvn_ratios_fmt(Str *out, WhvnRatios ratios) {
    ASSERT_ARG(out);
    size_t len = array_len(ratios);
    for(size_t i = 0; i < len; ++i) {
        if(i) str_push(out, ',');
        whvn_ratio_fmt(out, array_at(ratios, i));
    }
}

WhvnRatio whvn_ratio_parse(Str str) {
    Str h, w = str_split_ch(str, 'x', &h);
    if(!h.len) {
        if(!str_cmp(str, str("landscape"))) return (WhvnRatio){ .type = WHVN_RATIO_LANDSCAPE };
        if(!str_cmp(str, str("portrait"))) return (WhvnRatio){ .type = WHVN_RATIO_PORTRAIT };
    }
    WhvnRatio result = {0};
    int err = 0;
    err |= str_as_int(h, (int *)&result.h, 10);
    err |= str_as_int(w, (int *)&result.w, 10);
    if(err) return (WhvnRatio){0};
    return result;
}

