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
    str_free(&api->curl.websafe_url);
    curl_easy_cleanup(api->curl.handle);
}

#define ERR_whvn_api_curl_do(...) "curl failed"
ErrDecl whvn_api_curl_do(WhvnApi *api, Str url, Str *response) {
    whvn_api_curl_init(api);
    Str old = *response;
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(api->curl.handle, CURLOPT_URL, url.str);
    if(api->print_url) {
        printf("%.*s\n", STR_F(url));
    }
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl, error: %u", res);
    if(api->print_response) {
        printf("%.*s\n", STR_F(str_i0(*response, old.len)));
    }
    return 0;
error:
    return -1;
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

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, Str *buf, WhvnWallpaperInfo *info) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/w/", STR_F(str_ensure_dir(api->url)));
    str_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnWallpaperInfo result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_data_wallpaper_info, &result));
    *info = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, Str *buf, WhvnResponse *response) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/search/", STR_F(str_ensure_dir(api->url)));
    if(whvn_api_key_extend(&url, api)) {
        str_extend(&url, str("&"));
    } else {
        str_extend(&url, str("?"));
    }
    whvn_api_search_fmt_websafe(&url, arg);
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnResponse result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_response, &result));
    *response = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, Str *buf, WhvnTag *tag_info) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/tag/", STR_F(str_ensure_dir(api->url)));
    str_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnTag result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_data_tag_info, &result));
    *tag_info = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_settings(WhvnApi *api, Str *buf, WhvnUserSettings *settings) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/settings/", STR_F(str_ensure_dir(api->url)));
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnUserSettings result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_data_user_settings, &result));
    *settings = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collections(WhvnApi *api, size_t page, Str *buf, WhvnUserCollections *collections) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/collections/", STR_F(str_ensure_dir(api->url)));
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    if(page > 0) str_fmt(&url, "&page=%zu", page);
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnUserCollections result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_data_user_collections, &result));
    *collections = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collection(WhvnApi *api, size_t page, Str username, unsigned long id, Str *buf, WhvnResponse *response) {
    int err = 0;
    Str url = STR_DYN();
    str_fmt(&url, "%.*s/collections/%.*s/%lu", STR_F(str_ensure_dir(api->url)), STR_F(username), id);
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    if(page > 0) str_fmt(&url, "&page=%zu", page);
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnResponse result = {0};
    TRYC(json_parse(str_trim(*buf), whvn_json_parse_response, &result));
    *response = result;
clean:
    str_free(&url);
    return err;
error:
    ERR_CLEAN;
}

