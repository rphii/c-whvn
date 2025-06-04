#ifndef WHVN_USER_SETTINGS_H

#include <rphii/str.h>
#include "whvn-purity.h"
#include "whvn-category.h"

typedef struct WhvnUserSettings {
    Str                 thumb_size;
    unsigned long       per_page;
    WhvnCategoryFlag    categories;
    VStr                resolutions;
    VStr                aspect_ratios;
    Str                 toplist_range;
    VStr                tag_blacklist;
    VStr                user_blacklist;
} WhvnUserSettings;

#define WHVN_USER_SETTINGS_H
#endif

