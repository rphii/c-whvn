#ifndef WHVN_API_H

#include <rphii/err.h>
#include <curl/curl.h>
#include "whvn-collection.h"
#include "whvn-user-settings.h"
#include "whvn-user-collection.h"
#include "whvn-response.h"
#include "whvn-api-search.h"

#define WHVN_API_V1

#if defined(WHVN_API_V1)

#define WHVN_API_RATE_US   1450000

typedef struct WhvnApi {
    Str                 url;
    Str                 key;
    bool                print_url;
    bool                print_response;
    struct {
        CURL            *handle;
        bool            initialized;
        Str             websafe_url;
    } curl;
    /* TODO : time information to not hit API rate limit */
} WhvnApi;

void whvn_api_curl_init(WhvnApi *api);
void whvn_api_free(WhvnApi *api);

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, Str *buf, WhvnWallpaperInfo *info);
ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, Str *buf, WhvnResponse *response);
ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, Str *buf, WhvnTag *tag_info);
ErrDecl whvn_api_user_settings(WhvnApi *api, Str *buf, WhvnUserSettings *settings);
ErrDecl whvn_api_user_collections(WhvnApi *api, size_t page, Str *buf, WhvnUserCollections *collections);
ErrDecl whvn_api_user_collection(WhvnApi *api, size_t page, Str user, unsigned long id, Str *buf, WhvnResponse *response);
#endif /* WHVN_API_V1 */

#define WHVN_API_H
#endif

