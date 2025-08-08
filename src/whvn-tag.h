#ifndef WHVN_TAG_H

#include <rlso.h>
#include "whvn-purity.h"

typedef struct WhvnTag {
    unsigned long id;
    So name;
    So alias;
    unsigned long category_id;
    So category;
    WhvnPurity purity;
    So created_at;
} WhvnTag, *WhvnTags;

void whvn_tag_free(WhvnTag *tag);

#define WHVN_TAG_H
#endif


