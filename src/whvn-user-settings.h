#ifndef WHVN_USER_SETTINGS_H

#include <rlso.h>
#include "whvn-purity.h"
#include "whvn-category.h"
#include "whvn-toplist-range.h"

typedef struct WhvnUserSettings {
    So                      thumb_size;
    unsigned long           per_page;
    WhvnCategory            categories;
    WhvnPurity              purity;
    VSo                     resolutions;
    VSo                     aspect_ratios; /* TODO use whvn-resolution */
    WhvnToplistRangeList    toplist_range;
    VSo                     tag_blacklist;
    VSo                     user_blacklist;
    bool                    ai_art_filter;
} WhvnUserSettings;

void whvn_user_settings_free(WhvnUserSettings *settings);

#define WHVN_USER_SETTINGS_H
#endif

