#ifndef WHVN_PURITY_H

#include <stdbool.h>

typedef struct WhvnPurity {
    bool sfw;
    bool sketchy;
    bool nsfw;
} WhvnPurity;

#include <rphii/str.h>

Str whvn_purity_str(WhvnPurity f);
bool whvn_purity_is_set(WhvnPurity f);

#define WHVN_PURITY_H
#endif

