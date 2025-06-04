#ifndef WHVN_CLI_H

#include <rphii/arg.h>
#include "whvn-collection.h"
#include "whvn-arg.h"
#include "whvn-api.h"

typedef struct WhvnCli {
    struct Arg *arg;
    WhvnApi api;
    struct {
        Str wallpaper_info;
        Str search;
        Str tag_info;
    } query;
    struct {
        WhvnWallpaperInfo wallpaper_info;
        WhvnCollection search;
        WhvnTags tag_info;
        WhvnUserSettings user_settings;
        WhvnUserCollections user_collections;
    } result;
} WhvnCli;

#define WHVN_CLI_H
#endif

