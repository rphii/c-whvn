#include "whvn-purity.h"

Str whvn_purity_str(WhvnPurityFlag f) {
    switch(f) {
        case 0: return str("(no purity)");
        case WHVN_PURITY_SFW: return str("sfw");
        case WHVN_PURITY_NSFW: return str("nsfw");
        case WHVN_PURITY_SKETCHY: return str("sketchy");
        case (WHVN_PURITY_SFW|WHVN_PURITY_NSFW): return str("sfw,nsfw");
        case (WHVN_PURITY_NSFW|WHVN_PURITY_SKETCHY): return str("nsfw,sketchy");
        case (WHVN_PURITY_SKETCHY|WHVN_PURITY_SFW): return str("sfw,sketchy");
        case (WHVN_PURITY_SKETCHY|WHVN_PURITY_SFW|WHVN_PURITY_NSFW): return str("sfw,sketchy,nsfw");
        default: return str("???");
    }
}

