#ifndef WHVN_WALLPAPER_INFO_H

#include <rlso.h>
#include <rlc/color.h>
#include "whvn-avatar.h"
#include "whvn-uploader.h"
#include "whvn-purity.h"
#include "whvn-category.h"
#include "whvn-thumbs.h"
#include "whvn-tag.h"

typedef struct WhvnWallpaperInfo {
    So                  id;
    So                  url;
    So                  short_url;
    WhvnAvatar          avatar;
    WhvnUploader        uploader;
    unsigned long       views;
    unsigned long       favorites;
    So                  source;
    WhvnPurity          purity;
    WhvnCategory        category;
    unsigned long       dimension_x;
    unsigned long       dimension_y;
    So                  resolution;
    So                  ratio;
    unsigned long       file_size;
    So                  file_type; /*mime?*/
    So                  created_at; /*date?*/
    Color               *colors;
    So                  path;
    WhvnThumbs          thumbs;
    WhvnTags            tags;
} WhvnWallpaperInfo, *WhvnWallpaperInfos;

void whvn_wallpaper_info_free(WhvnWallpaperInfo *wp);
void whvn_wallpaper_info_print(WhvnWallpaperInfo *wp);

#define WHVN_WALLPAPER_INFO_H
#endif

