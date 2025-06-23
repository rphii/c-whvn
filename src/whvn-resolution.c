#include "whvn-resolution.h"

bool whvn_resolution_is_valid(WhvnResolution res) {
    bool result = res.w && res.h;
    return result;
}

void whvn_resolution_fmt(Str *out, WhvnResolution res) {
    ASSERT_ARG(out);
    str_fmt(out, "%ux%u", res.w, res.h);
}

void whvn_resolutions_fmt(Str *out, WhvnResolutions res) {
    ASSERT_ARG(out);
    size_t len = array_len(res);
    size_t valid = 0;
    for(size_t i = 0; i < len; ++i) {
        WhvnResolution r = array_at(res, i);
        if(valid) str_push(out, ',');
        if(!whvn_resolution_is_valid(r)) continue;
        whvn_resolution_fmt(out, array_at(res, i));
        ++valid;
    }
}

WhvnResolution whvn_resolution_from_str(Str s) {
    WhvnResolution result = {0};
    Str right, left = str_split_ch(s, 'x', &right);
    return result;
}

