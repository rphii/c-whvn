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
    Str api_response = whvn_api_curl_do(api, url);
    json_parse(str_trim(api_response), whvn_json_parse_data_wallpaper_info, info);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, WhvnResponse *response) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/search"));
    if(whvn_api_key_extend(&url, api)) {
        str_extend(&url, str("&"));
    } else {
        str_extend(&url, str("?"));
    }
    whvn_api_search_fmt_websafe(&url, arg);
    Str api_response = whvn_api_curl_do(api, url);
    json_parse(str_trim(api_response), whvn_json_parse_response, response);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, WhvnTag *tag_info) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/tag/"));
    str_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    Str response = whvn_api_curl_do(api, url);
    json_parse(str_trim(response), whvn_json_parse_data_tag_info , tag_info);
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
    json_parse(str_trim(response), whvn_json_parse_data_user_settings , settings);
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
    Str api_response = whvn_api_curl_do(api, url);
    json_parse(str_trim(api_response), whvn_json_parse_data_user_collections, collections);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collection(WhvnApi *api, Str arg, WhvnResponse *response) {
    int err = 0;
    Str url = str_dyn(STR("https://wallhaven.cc/api/v1/collections/"));
    str_extend(&url, arg);
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    Str api_response = whvn_api_curl_do(api, url);
    json_parse(str_trim(api_response), whvn_json_parse_response, response);
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

