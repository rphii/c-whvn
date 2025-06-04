#include "whvn-api.h"

void whvn_api_curl_init(WhvnApi *api) {
    ASSERT_ARG(api);
    if(api->curl.initialized) return;
    api->curl.handle = curl_easy_init();
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEFUNCTION, str_writefunc);
    api->curl.initialized = true;
}

void whvn_api_free(WhvnApi *api) {
    array_free_set(api->responses, Str, (ArrayFree)str_free);
    array_free(api->responses);
    curl_easy_cleanup(api->curl.handle);
}

Str whvn_api_curl_do(WhvnApi *api, Str url) {
    whvn_api_curl_init(api);
    size_t len = array_len(api->responses);
    array_resize(api->responses, len + 1);
    Str *it = array_it(api->responses, len);
    //printf("WRITEBACK:%p\n", it);
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEDATA, it);
    curl_easy_setopt(api->curl.handle, CURLOPT_URL, url.str);
    //printf("CURL-URL:%.*s\n", STR_F(url));
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl: %u", res);
    //printf("RESPONSE:%.*s\n", STR_F(*it));
error:
    return *it;
}

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, WhvnWallpaperInfo *info) {
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/w/"));
    str_extend(&url, arg);
    Str response = whvn_api_curl_do(api, url);
    printf("RESPONSE:%.*s\n", STR_F(response));
    str_free(&url);
    return 0;
}

ErrDecl whvn_api_search(WhvnApi *api, Str arg, WhvnCollection *collection) {
}

ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, WhvnTags *tags) {
}

ErrDecl whvn_api_user_settings(WhvnApi *api, Str arg, WhvnUserSettings *settings) {
}

ErrDecl whvn_api_user_collections(WhvnApi *api, Str arg, WhvnUserCollections *collections) {
}



