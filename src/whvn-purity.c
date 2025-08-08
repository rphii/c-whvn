#include "whvn-purity.h"

So whvn_purity_str(WhvnPurity f) {
    if(f.sketchy && f.sfw && f.nsfw) return so("sfw,sketchy,nsfw");
    if(f.sfw && f.nsfw) return so("sfw,nsfw");
    if(f.nsfw && f.sketchy) return so("nsfw,sketchy");
    if(f.sketchy && f.sfw) return so("sfw,sketchy");
    if(f.sfw) return so("sfw");
    if(f.nsfw) return so("nsfw");
    if(f.sketchy) return so("sketchy");
    return so("(no purity)");
}

bool whvn_purity_is_set(WhvnPurity f) {
    return f.sfw || f.sketchy || f.nsfw;
}

