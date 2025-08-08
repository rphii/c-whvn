#ifndef WHVN_COLLECTION

#include <rlc/lut.h>
#include "whvn-wallpaper-info.h"

LUT_INCLUDE(TWhvnWallpaperInfo, twhvn_wallpaper_info, So, BY_REF, WhvnWallpaperInfo, BY_REF);

typedef struct WhvnCollection {
    TWhvnWallpaperInfoKV **vec;
    TWhvnWallpaperInfo map;
} WhvnCollection;

void whvn_collection_init(WhvnCollection *self);
void whvn_collection_add(WhvnCollection *self, WhvnWallpaperInfo *wp);
void whvn_collection_free(WhvnCollection *self);

#define WHVN_COLLECTION
#endif

