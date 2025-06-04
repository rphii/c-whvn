#ifndef WHVN_WALLPAPER_INFO_H

#include <rphii/str.h>
#include <rphii/color.h>
#include "whvn-avatar.h"
#include "whvn-uploader.h"
#include "whvn-purity.h"
#include "whvn-category.h"
#include "whvn-thumbs.h"
#include "whvn-tag.h"

typedef struct WhvnWallpaperInfo {
    Str                 id;
    Str                 url;
    Str                 short_url;
    WhvnAvatar          avatar;
    WhvnUploader        uploader;
    unsigned long       views;
    unsigned long       favorites;
    Str                 source;
    WhvnPurityFlag      purity;
    WhvnCategoryFlag    categories;
    unsigned long       dimension_x;
    unsigned long       dimension_y;
    Str                 resolution;
    double              ratio;
    unsigned long       file_size;
    Str                 file_type; /*mime?*/
    Str                 created_at; /*date?*/
    Color               *colors;
    Str                 path;
    WhvnThumbs          thumbs;
    WhvnTags            tags;
} WhvnWallpaperInfo;

void whvn_wallpaper_info_free(WhvnWallpaperInfo *wp);

#define WHVN_WALLPAPER_INFO_H
#endif

