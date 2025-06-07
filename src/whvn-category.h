#ifndef WHVN_CATEGORY_H

typedef enum {
    WHVN_CATEGORY_GENERAL = 0x1,
    WHVN_CATEGORY_ANIME   = 0x2,
    WHVN_CATEGORY_PEOPLE  = 0x4,
} WhvnCategoryFlag;

#include <rphii/str.h>

Str whvn_category_str(WhvnCategoryFlag f);

#define WHVN_CATEGORY_H
#endif

