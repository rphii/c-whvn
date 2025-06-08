#include "whvn-category.h"

Str whvn_category_str(WhvnCategory f) {
    if(f.general && f.people && f.anime) return str("general,anime,people");
    if(f.people && f.anime) return str("anime,people");
    if(f.anime && f.general) return str("general,anime");
    if(f.general && f.people) return str("general,people");
    if(f.people) return str("people");
    if(f.anime) return str("anime");
    if(f.general) return str("general");
    return str("(no category)");
}

bool whvn_category_is_set(WhvnCategory f) {
    return f.general || f.people || f.anime;
}

