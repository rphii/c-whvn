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

/*
# %i  id : str
# %u  url : str
# %U  short_url : str
# %0  avatar.200 : str
# %8  avatar.128 : str
# %3  avatar.32 : str
# %2  avatar.20 : str
# %@n uploader.username : str
# %@g uploader.group : str
# %@0 uploader.avatar.200 : str
# %@8 uploader.avatar.128 : str
# %@3 uploader.avatar.32 : str
# %@2 uploader.avatar.20 : str
# %v  views : int
# %f  favorites : int
# %s  source : str
# %p  purity : str
#     %p:s  string : e.g. sfw,sketchy,nsfw
#     %p:S  string : e.g. SFW,SKETCHY,NSFW
#     %p:b  binary : e.g. 111
#     %p:c  character : e.g. SZN
# %c  category : str
#     %c:s  string : e.g. general,anime,people
#     %c:S  string : e.g. GENERAL,ANIME,PEOPLE
#     %c:b  binary : e.g. 111
#     %c:c  character : e.g. GAP
# %D  dimension : str ??
# %w  resolution.w : int
# %h  resolution.h : int
# %W  ratio.w : int
# %H  ratio.h : int
# %S  file_size : int
# %T  file_type : str
# %d  created_at : str
# %C[..]  colors : color[]
#    %r   red : %02x
#    %g   green : %02x
#    %b   blue %02x
#    %R   red : %u
#    %G   green : %u
#    %B   blue : %u
#    %rgb rgb : %02%02xx%02x
#    %RGB rgb : %u,%u,%u

    "   %C[#%rgb ]  "
    "   %C[{  +%rgb}]  "

# %P  path : str
# %L  thumbs.large : str
# %O  thumbs.original : str
# %l  thumbs.small : str
# %t[..]  tags : tags[]
#    %i   tags[].id : int
#    %n   tags[].name : str
#    %a   tags[].alias : str
#    %I   tags[].category_id : int
#    %c   tags[].category : str
#    %p   tags[].purity : str
#    %ps  tags[].purity.sfw : bool
#    %py  tags[].purity.sketchy : bool
#    %pn  tags[].purity.nsfw : bool
#    %d   tags[].created_at : str

format structs!? a thing's fmt can be overridden by another ones?
*/
void whvn_wallpaper_info_fmt(Str *out, const char *fmt, WhvnWallpaperInfo *wp) {
    ASSERT_ARG(out);
    if(!wp) return;
    Str f = str_l(fmt);
    size_t i = 0;
    for(Str splice = {0}; str_splice(f, &splice, '%'); ++i) {
        if(!splice.str) continue;
        if(!i) {
            str_extend(out, splice);
        } else if(splice.len) {
            char c = str_at(splice, 0);
            switch(c) {
                case 'i': { str_fmt(out, "%lu", wp->id); } break;
                case 'u': { str_fmt(out, "%.*s", STR_F(wp->url)); } break;
                case 'U': { str_fmt(out, "%.*s", STR_F(wp->short_url)); } break;
                case '0': { str_fmt(out, "%.*s", STR_F(wp->avatar.px200)); } break;
                case '8': { str_fmt(out, "%.*s", STR_F(wp->avatar.px128)); } break;
                case '3': { str_fmt(out, "%.*s", STR_F(wp->avatar.px32)); } break;
                case '2': { str_fmt(out, "%.*s", STR_F(wp->avatar.px20)); } break;
                case '@': {
                    if(splice.len < 2) break;
                    char d = str_at(splice, 1);
                    switch (d) {
                        case 'n': { str_fmt(out, "%.*s", STR_F(wp->uploader.username)); } break;
                        case 'g': { str_fmt(out, "%.*s", STR_F(wp->uploader.group)); } break;
                        case '0': { str_fmt(out, "%.*s", STR_F(wp->uploader.avatar.px200)); } break;
                        case '8': { str_fmt(out, "%.*s", STR_F(wp->uploader.avatar.px128)); } break;
                        case '3': { str_fmt(out, "%.*s", STR_F(wp->uploader.avatar.px32)); } break;
                        case '2': { str_fmt(out, "%.*s", STR_F(wp->uploader.avatar.px20)); } break;
                        default: break;
                    }
                } break;
                case 'v': { str_fmt(out, "%lu", wp->views); } break;
                case 's': { str_fmt(out, "%.*s", STR_F(wp->source)); } break;
                case 'f': { str_fmt(out, "%lu", wp->favorites); } break;
                case 'p': { str_fmt(out, "%.*s", STR_F(wp->path)); } break;
            }
        }
    }
}

