#include "whvn-tag.h"

void whvn_tag_free(WhvnTag *tag) {
    ASSERT_ARG(tag);
    str_free(&tag->name);
    str_free(&tag->alias);
    str_free(&tag->category);
    str_free(&tag->created_at);
#ifndef NDEBUG
    memset(tag, 0, sizeof(*tag));
#endif
}

