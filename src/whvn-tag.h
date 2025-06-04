#ifndef WHVN_TAG_H

#include <rphii/str.h>
#include "whvn-purity.h"

typedef struct WhvnTag {
    unsigned long id;
    Str name;
    Str alias;
    Str category_id;
    Str category;
    WhvnPurityFlag purity;
    Str created_at;
} WhvnTag, *WhvnTags;

#define WHVN_TAG_H
#endif


