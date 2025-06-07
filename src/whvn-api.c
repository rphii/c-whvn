#include "whvn-api.h"
#include "whvn-json-parse.h"

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
    str_free(&api->curl.websafe_url);
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
    if(api->print_url) {
        printf("%.*s\n", STR_F(url));
    }
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl: %u", res);
    if(api->print_response) {
        printf("%.*s\n", STR_F(*it));
    }
error:
    return *it;
}

#define ERR_whvn_api_key_extend(...) "failed getting API key"
int whvn_api_key_extend(Str *out, WhvnApi *api) {
    ASSERT_ARG(out);
    ASSERT_ARG(api);
    if(str_len(api->key)) {
        str_extend(out, str("?apikey="));
        str_extend(out, api->key);
        return true;
    }
    return false;
}

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, WhvnWallpaperInfo *info) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/w/"));
    str_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    Str response = whvn_api_curl_do(api, url);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_search(WhvnApi *api, Str arg, WhvnCollection *collection) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/search"));
    if(whvn_api_key_extend(&url, api)) {
        str_extend(&url, str("&"));
    } else {
        str_extend(&url, str("?"));
    }
    str_extend(&url, str("q="));
    str_fmt_websafe(&url, arg);
    Str response = whvn_api_curl_do(api, url);
    WhvnResponse parsed = {0};
    json_parse(str_trim(response), whvn_json_parse_response, &parsed);
    Str color_s = STR_DYN();
    for(size_t i = 0; i < array_len(parsed.data); ++i) {
        WhvnWallpaperInfo info = array_at(parsed.data, i);
        printf("%6zu %.*s " F("%.*s", UL FG_BL) " ", i, STR_F(info.id), STR_F(info.url));
        for(size_t j = 0; j < array_len(info.colors); ++j) {
            Color color = array_at(info.colors, j);
            str_clear(&color_s);
            if(color.rgba == 0x00) color.r = 0x01;
            color_fmt_rgb_fmt(&color_s, color, str("  "));
            printf("%.*s", STR_F(color_s));
        }
        printf(" " F("%lu 󰣐 ", FG_RD_B) "", info.favorites);
        printf("\n");
    }
clean:
    whvn_response_free(&parsed);
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, WhvnTags *tags) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/tag/"));
    str_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    Str response = whvn_api_curl_do(api, url);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_settings(WhvnApi *api, Str arg, WhvnUserSettings *settings) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/settings/"));
    str_extend(&url, arg);
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    Str response = whvn_api_curl_do(api, url);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collections(WhvnApi *api, Str arg, WhvnUserCollections *collections) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/collections/"));
    str_extend(&url, arg);
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    Str response = whvn_api_curl_do(api, url);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

