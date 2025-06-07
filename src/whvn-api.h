#ifndef WHVN_API_H

#include <rphii/err.h>
#include <curl/curl.h>
#include "whvn-collection.h"
#include "whvn-user-settings.h"
#include "whvn-user-collection.h"
#include "whvn-meta.h"
#include "whvn-response.h"

#define WHVN_API_V1

#if defined(WHVN_API_V1)
typedef struct WhvnApi {
    Str key;
    VStr responses;
    bool print_url;
    bool print_response;
    bool print_pretty;
    struct {
        CURL *handle;
        bool initialized;
        Str websafe_url;
    } curl;
    WhvnWallpaperInfo wallpaper_info;
    WhvnCollection collection;
    WhvnTags tags;
    WhvnUserSettings user_settings;
} WhvnApi;
#include "whvn.h"

void whvn_api_curl_init(WhvnApi *api);
void whvn_api_free(WhvnApi *api);

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, WhvnWallpaperInfo *info);
ErrDecl whvn_api_search(WhvnApi *api, Str arg, WhvnCollection *collection);
ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, WhvnTags *tags);
ErrDecl whvn_api_user_settings(WhvnApi *api, Str arg, WhvnUserSettings *settings);
ErrDecl whvn_api_user_collections(WhvnApi *api, Str arg, WhvnUserCollections *collections);
ErrDecl whvn_api_user_collection(WhvnApi *api, Str arg, WhvnWallpaperInfo *collections);

#endif

#define WHVN_API_H
#endif

