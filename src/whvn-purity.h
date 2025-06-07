#ifndef WHVN_PURITY_H

typedef enum {
    WHVN_PURITY_SFW     = 0x1,
    WHVN_PURITY_SKETCHY = 0x4,
    WHVN_PURITY_NSFW    = 0x2,
} WhvnPurityFlag;

#include <rphii/str.h>

Str whvn_purity_str(WhvnPurityFlag f);

#define WHVN_PURITY_H
#endif

