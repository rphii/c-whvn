#include "whvn-category.h"

Str whvn_category_str(WhvnCategoryFlag f) {
    if(!f) return str("(no category)");
    if(f == WHVN_CATEGORY_PEOPLE) return str("people");
    if(f == WHVN_CATEGORY_ANIME) return str("anime");
    if(f == WHVN_CATEGORY_GENERAL) return str("general");
    if(f == (WHVN_CATEGORY_PEOPLE|WHVN_CATEGORY_ANIME)) return str("anime,people");
    if(f == (WHVN_CATEGORY_ANIME|WHVN_CATEGORY_GENERAL)) return str("general,anime");
    if(f == (WHVN_CATEGORY_GENERAL|WHVN_CATEGORY_PEOPLE)) return str("general,people");
    if(f == (WHVN_CATEGORY_GENERAL|WHVN_CATEGORY_PEOPLE|WHVN_CATEGORY_ANIME)) return str("general,anime,people");
    return str("???");
}

