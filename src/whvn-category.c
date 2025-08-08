#include "whvn-category.h"

So whvn_category_str(WhvnCategory f) {
    if(f.general && f.people && f.anime) return so("general,anime,people");
    if(f.people && f.anime) return so("anime,people");
    if(f.anime && f.general) return so("general,anime");
    if(f.general && f.people) return so("general,people");
    if(f.people) return so("people");
    if(f.anime) return so("anime");
    if(f.general) return so("general");
    return so("(no category)");
}

bool whvn_category_is_set(WhvnCategory f) {
    return f.general || f.people || f.anime;
}

