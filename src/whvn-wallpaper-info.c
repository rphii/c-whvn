#include "whvn-wallpaper-info.h"

void whvn_wallpaper_info_free(WhvnWallpaperInfo *wp) {
    ASSERT_ARG(wp);
    array_free(wp->colors);
    array_free(wp->tags);
#ifndef NDEBUG
    memset(wp, 0, sizeof(*wp));
#endif
}

void whvn_wallpaper_info_print(WhvnWallpaperInfo *wp) {
    Str color_s = STR_DYN();
    printf("id %.*s\n", STR_F(wp->id));
    printf("url %.*s\n", STR_F(wp->url));
    printf("short_url %.*s\n", STR_F(wp->short_url));
    printf("uploader.username %.*s\n", STR_F(wp->uploader.username));
    printf("uploader.group %.*s\n", STR_F(wp->uploader.group));
    printf("uploader.avatar.200px %.*s\n", STR_F(wp->uploader.avatar.px200));
    printf("uploader.avatar.128px %.*s\n", STR_F(wp->uploader.avatar.px128));
    printf("uploader.avatar.32px %.*s\n", STR_F(wp->uploader.avatar.px32));
    printf("uploader.avatar.20px %.*s\n", STR_F(wp->uploader.avatar.px20));
    printf("views %zu\n", wp->views);
    printf("favorites  %zu\n", wp->favorites);
    printf("source %.*s\n", STR_F(wp->source));
    printf("purity %.*s\n", STR_F(whvn_purity_str(wp->purity)));
    printf("category %.*s\n", STR_F(whvn_category_str(wp->category)));
    printf("dimension_x %zu\n", wp->dimension_x);
    printf("dimension_y %zu\n", wp->dimension_y);
    printf("resolution %.*s\n", STR_F(wp->resolution));
    printf("ratio %.*s\n", STR_F(wp->ratio));
    printf("file_size %zu\n", wp->file_size);
    printf("file_type %.*s\n", STR_F(wp->file_type));
    printf("created_at %.*s\n", STR_F(wp->created_at));
    printf("colors len %zu", array_len(wp->colors));
    for(size_t j = 0; j < array_len(wp->colors); ++j) {
        str_clear(&color_s);
        Color color = array_at(wp->colors, j);
        color_fmt_rgb(&color_s, color);
        printf(" %.*s", STR_F(color_s));
    }
    printf("\n");
    for(size_t j = 0; j < array_len(wp->tags); ++j) {
        WhvnTag tag = array_at(wp->tags, j);
        printf(" tag.id %lu name %.*s alias %.*s category_id %lu category %.*s purity %.*s created_at %.*s\n",
                tag.id, STR_F(tag.name), STR_F(tag.alias), tag.category_id, STR_F(tag.category),
                STR_F(whvn_purity_str(tag.purity)), STR_F(tag.created_at));
    }
    printf("path %.*s\n", STR_F(wp->path));
    printf("thumbs.small %.*s\n", STR_F(wp->thumbs.small));
    printf("thumbs.large %.*s\n", STR_F(wp->thumbs.large));
    printf("thumbs.original %.*s\n", STR_F(wp->thumbs.original));
    str_free(&color_s);
}

void whvn_wallpaper_info_fmt(Str *out, const char fmt, WhvnWallpaperInfo *wp) {
    /*
id : str
url : str
short_url : str
avatar.200 : str
avatar.128 : str
avatar.32 : str
avatar.20 : str
uploader.username : str
uploader.group : str
uploader.avatar.200 : str
uploader.avatar.128 : str
uploader.avatar.32 : str
uploader.avatar.20 : str
views : int
favorites : int
source : str
purity.sfw : bool
purity.sketchy : bool
purity.nsfw : bool
category.general : bool
category.anime : bool
category.people : bool
dimension.x : int
dimension.y : int
resolution.w : int
resolution.h : int
ratio.w : int
ratio.h : int
file_size : int
file_type : str
created_at : str
colors : color[]
path : str
thumbs.large : str
thumbs.original : str
thumbs.small : str
tags : tags[]
tags[].id : int
tags[].name : str
tags[].alias : str
tags[].category_id : int
tags[].category : str
tags[].purity.sfw : bool
tags[].purity.sketchy : bool
tags[].purity.nsfw : bool
tags[].created_at : str
    */
}

