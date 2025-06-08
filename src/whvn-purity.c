#include "whvn-purity.h"

Str whvn_purity_str(WhvnPurity f) {
    if(f.sketchy && f.sfw && f.nsfw) return str("sfw,sketchy,nsfw");
    if(f.sfw && f.nsfw) return str("sfw,nsfw");
    if(f.nsfw && f.sketchy) return str("nsfw,sketchy");
    if(f.sketchy && f.sfw) return str("sfw,sketchy");
    if(f.sfw) return str("sfw");
    if(f.nsfw) return str("nsfw");
    if(f.sketchy) return str("sketchy");
    return str("(no purity)");
}

bool whvn_purity_is_set(WhvnPurity f) {
    return f.sfw || f.sketchy || f.nsfw;
}

