#include "whvn-category.h"

Str whvn_category_str(WhvnCategoryFlag f) {
    switch(f) {
        case 0: return str("(no category)");
        case WHVN_CATEGORY_PEOPLE: return str("people");
        case WHVN_CATEGORY_ANIME: return str("anime");
        case WHVN_CATEGORY_GENERAL: return str("general");
        case (WHVN_CATEGORY_PEOPLE|WHVN_CATEGORY_ANIME): return str("anime,people");
        case (WHVN_CATEGORY_ANIME|WHVN_CATEGORY_GENERAL): return str("general,anime");
        case (WHVN_CATEGORY_GENERAL|WHVN_CATEGORY_PEOPLE): return str("general,people");
        case (WHVN_CATEGORY_GENERAL|WHVN_CATEGORY_PEOPLE|WHVN_CATEGORY_ANIME): return str("general,anime,people");
        default: return str("???");
    }
}

