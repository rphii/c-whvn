#ifndef WHVN_CATEGORY_H

#include <stdbool.h>

typedef struct WhvnCategory {
    bool general;
    bool anime;
    bool people;
} WhvnCategory;

#include <rlso.h>

So whvn_category_str(WhvnCategory f);
bool whvn_category_is_set(WhvnCategory f);

#define WHVN_CATEGORY_H
#endif

