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

