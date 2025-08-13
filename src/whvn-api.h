#ifndef WHVN_API_H

#include <rlso.h>
#include <curl/curl.h>
#include "whvn-collection.h"
#include "whvn-user-settings.h"
#include "whvn-user-collection.h"
#include "whvn-response.h"
#include "whvn-api-search.h"

#define WHVN_API_V1

#if defined(WHVN_API_V1)

//#define WHVN_API_RATE_US   1450000
#define WHVN_API_RATE_US   2000000

typedef struct WhvnApi {
    So                  url;
    So                  key;
    bool                print_url;
    bool                print_response;
    struct {
        CURL            *handle;
        bool            initialized;
        So              websafe_url;
    } curl;
    /* TODO : time information to not hit API rate limit */
} WhvnApi;

void whvn_api_curl_init(WhvnApi *api);
void whvn_api_free(WhvnApi *api);

ErrDecl whvn_api_download(WhvnApi *api, WhvnWallpaperInfo *info, So filename);
ErrDecl whvn_api_wallpaper_info(WhvnApi *api, So arg, So *buf, WhvnWallpaperInfo *info);
ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, So *buf, WhvnResponse *response);
ErrDecl whvn_api_tag_info(WhvnApi *api, So arg, So *buf, WhvnTag *tag_info);
ErrDecl whvn_api_user_settings(WhvnApi *api, So *buf, WhvnUserSettings *settings);
ErrDecl whvn_api_user_collections(WhvnApi *api, size_t page, So *buf, WhvnUserCollections *collections);
ErrDecl whvn_api_user_collection(WhvnApi *api, size_t page, So user, unsigned long id, So *buf, WhvnResponse *response);
#endif /* WHVN_API_V1 */

#define WHVN_API_H
#endif

