#include "whvn-collection.h"
#include <rphii/array.h>

LUT_IMPLEMENT(TWhvnWallpaperInfo, twhvn_wallpaper_info, Str, BY_REF, WhvnWallpaperInfo, BY_REF, str_hash, str_hcmp, 0, 0);

void whvn_collection_init(WhvnCollection *self) {
    ASSERT_ARG(self);
    printff("initialized collection %p", self);
    //map_get(self->map, &STR("asdf"));
}

void whvn_collection_add(WhvnCollection *self, WhvnWallpaperInfo *wp) {
    ASSERT_ARG(self);
    ASSERT_ARG(wp);
    TWhvnWallpaperInfoKV *have = twhvn_wallpaper_info_get_kv(&self->map, &wp->id);
    if(!have) {
        have = twhvn_wallpaper_info_set(&self->map, &wp->id, wp);
        //printff("CREATED %p %.*s",have, STR_F(wp->id));
    } else {
        //printff("HAD %p %.*s",have, STR_F(wp->id));
    }
    ASSERT_ARG(have);
    array_push(self->vec, have);
}

void whvn_collection_free(WhvnCollection *self) {
    ASSERT_ARG(self);
    array_free(self->vec);
    twhvn_wallpaper_info_free(&self->map);
    memset(self, 0, sizeof(*self));
}

