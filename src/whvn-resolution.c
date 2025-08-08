#include "whvn-resolution.h"

bool whvn_resolution_is_valid(WhvnResolution res) {
    bool result = res.w && res.h;
    return result;
}

void whvn_resolution_fmt(So *out, WhvnResolution res) {
    ASSERT_ARG(out);
    so_fmt(out, "%ux%u", res.w, res.h);
}

void whvn_resolutions_fmt(So *out, WhvnResolutions res) {
    ASSERT_ARG(out);
    size_t len = array_len(res);
    size_t valid = 0;
    for(size_t i = 0; i < len; ++i) {
        WhvnResolution r = array_at(res, i);
        if(valid) so_push(out, ',');
        if(!whvn_resolution_is_valid(r)) continue;
        whvn_resolution_fmt(out, array_at(res, i));
        ++valid;
    }
}

WhvnResolution whvn_resolution_from_str(So s) {
    WhvnResolution result = {0};
    So right, left = so_split_ch(s, 'x', &right);
    return result;
}

WhvnResolution whvn_resolution_parse(So str) {
    So h, w = so_split_ch(str, 'x', &h);
    if(!h.len) {
        return (WhvnResolution){0};
    }
    WhvnResolution result = {0};
    int err = 0;
    err |= so_as_int(h, (int *)&result.h, 10);
    err |= so_as_int(w, (int *)&result.w, 10);
    if(err) return (WhvnResolution){0};
    return result;
}

