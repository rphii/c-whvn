#include "whvn-purity.h"

Str whvn_purity_str(WhvnPurityFlag f) {
    if(!f) return str("(no purity)");
    if(f == WHVN_PURITY_SFW) return str("sfw");
    if(f == WHVN_PURITY_NSFW) return str("nsfw");
    if(f == WHVN_PURITY_SKETCHY) return str("sketchy");
    if(f == (WHVN_PURITY_SFW|WHVN_PURITY_NSFW)) return str("sfw,nsfw");
    if(f == (WHVN_PURITY_NSFW|WHVN_PURITY_SKETCHY)) return str("nsfw,sketchy");
    if(f == (WHVN_PURITY_SKETCHY|WHVN_PURITY_SFW)) return str("sfw,sketchy");
    if(f == (WHVN_PURITY_SKETCHY|WHVN_PURITY_SFW|WHVN_PURITY_NSFW)) return str("sfw,sketchy,nsfw");
    return str("???");
}

