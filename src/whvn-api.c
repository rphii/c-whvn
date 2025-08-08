#include "whvn-api.h"
#include "whvn-json-parse.h"

void whvn_api_curl_init(WhvnApi *api) {
    ASSERT_ARG(api);
    if(api->curl.initialized) return;
    api->curl.handle = curl_easy_init();
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEFUNCTION, so_writefunc);
    api->curl.initialized = true;
}

void whvn_api_free(WhvnApi *api) {
    so_free(&api->curl.websafe_url);
    curl_easy_cleanup(api->curl.handle);
}

#define ERR_whvn_api_curl_do(...) "curl failed"
ErrDecl whvn_api_curl_do(WhvnApi *api, So url, So *response) {
    whvn_api_curl_init(api);
    So old = *response;
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(api->curl.handle, CURLOPT_URL, url.str);
    if(api->print_url) {
        so_println(url);
    }
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl, error: %u", res);
    if(api->print_response) {
        so_println(so_i0(*response, old.len));
    }
    return 0;
error:
    return -1;
}

#define ERR_whvn_api_key_extend(...) "failed getting API key"
int whvn_api_key_extend(So *out, WhvnApi *api) {
    ASSERT_ARG(out);
    ASSERT_ARG(api);
    if(so_len(api->key)) {
        so_extend(out, so("?apikey="));
        so_extend(out, api->key);
        return true;
    }
    return false;
}

ErrDecl whvn_api_wallpaper_info(WhvnApi *api, So arg, So *buf, WhvnWallpaperInfo *info) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/w/", SO_F(so_ensure_dir(api->url)));
    so_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnWallpaperInfo result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_data_wallpaper_info, &result));
    *info = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, So *buf, WhvnResponse *response) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/search/", SO_F(so_ensure_dir(api->url)));
    if(whvn_api_key_extend(&url, api)) {
        so_extend(&url, so("&"));
    } else {
        so_extend(&url, so("?"));
    }
    whvn_api_search_fmt_websafe(&url, arg);
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnResponse result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_response, &result));
    *response = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_tag_info(WhvnApi *api, So arg, So *buf, WhvnTag *tag_info) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/tag/", SO_F(so_ensure_dir(api->url)));
    so_extend(&url, arg);
    whvn_api_key_extend(&url, api);
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnTag result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_data_tag_info, &result));
    *tag_info = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_settings(WhvnApi *api, So *buf, WhvnUserSettings *settings) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/settings/", SO_F(so_ensure_dir(api->url)));
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnUserSettings result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_data_user_settings, &result));
    *settings = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collections(WhvnApi *api, size_t page, So *buf, WhvnUserCollections *collections) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/collections/", SO_F(so_ensure_dir(api->url)));
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    if(page > 0) so_fmt(&url, "&page=%zu", page);
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnUserCollections result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_data_user_collections, &result));
    *collections = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

ErrDecl whvn_api_user_collection(WhvnApi *api, size_t page, So username, unsigned long id, So *buf, WhvnResponse *response) {
    int err = 0;
    So url = SO;
    so_fmt(&url, "%.*s/collections/%.*s/%lu", SO_F(so_ensure_dir(api->url)), SO_F(username), id);
    if(!whvn_api_key_extend(&url, api)) THROW("API key required but not set");
    if(page > 0) so_fmt(&url, "&page=%zu", page);
    size_t i0 = buf->len;
    TRYC(whvn_api_curl_do(api, url, buf));
    WhvnResponse result = {0};
    TRYC(json_parse(so_trim(so_i0(*buf, i0)), whvn_json_parse_response, &result));
    *response = result;
clean:
    so_free(&url);
    return err;
error:
    ERR_CLEAN;
}

