#ifndef WHVN_TAG_H

#include <rphii/str.h>
#include "whvn-purity.h"

typedef struct WhvnTag {
    unsigned long id;
    Str name;
    Str alias;
    unsigned long category_id;
    Str category;
    WhvnPurityFlag purity;
    Str created_at;
} WhvnTag, *WhvnTags;

void whvn_tag_free(WhvnTag *tag);

#define WHVN_TAG_H
#endif


