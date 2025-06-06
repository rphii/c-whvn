#ifndef WHVN_RESPONSE

#include "whvn-wallpaper-info.h"
#include "whvn-meta.h"

typedef struct WhvnResponse {
    WhvnWallpaperInfo *data;
    WhvnMeta meta;
} WhvnResponse;

#define WHVN_RESPONSE
#endif

