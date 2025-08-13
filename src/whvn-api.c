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

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) { /*{{{*/
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
} /*}}}*/

#define ERR_whvn_api_curl_do(...) "curl failed"
ErrDecl whvn_api_curl_do(WhvnApi *api, So url, So *response) {
    whvn_api_curl_init(api);
    int err = 0;
    So old = *response;
    char *curl = so_dup(url);
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEFUNCTION, so_writefunc);
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(api->curl.handle, CURLOPT_URL, curl);
    if(api->print_url) {
        so_println(url);
    }
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl, error: %u", res);
    if(api->print_response) {
        so_println(so_i0(*response, old.len));
    }
clean:
    free(curl);
    return err;
error:
    ERR_CLEAN;
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

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

ErrDecl whvn_api_download(WhvnApi *api, WhvnWallpaperInfo *info, So filename) {
    whvn_api_curl_init(api);

    int err = 0;
    FILE *fp = 0;
    So path = info->path;
    char *curl = so_dup(path);

    So_File_Type_List type = so_file_get_type(filename);
    if(type == SO_FILE_TYPE_FILE) {
        size_t size = so_file_get_size(filename);
        if(size == info->file_size) {
        //printff(" %.*s -> [%.*s]",SO_F(path),SO_F(filename));
            return 0;
        }
    }

    //printff("  [%.*s] -> [%.*s]",SO_F(path),SO_F(filename));

    usleep(WHVN_API_RATE_US);
    char cdir[SO_FILE_PATH_MAX];
    So subdirs = SO;
    for(So subdir = SO; so_splice(filename, &subdir, PLATFORM_CH_SUBDIR); ) {
        if(so_is_zero(subdir)) continue;
        if(so_len(subdirs)) {
            so_as_cstr(subdirs, cdir, SO_FILE_PATH_MAX);
            errno = 0;
            mkdir(cdir, 0700);
            if(errno && errno != EEXIST) {
                THROW("failed creating folder: %.*s", SO_F(subdirs));
            }
        }
        so_path_join(&subdirs, subdirs, subdir);
    }

    fp = so_file_fp(filename, "w");
    if(!fp) THROW("failed opening file: %.*s", SO_F(filename));
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEFUNCTION, (curl_write_callback) write_data);
    curl_easy_setopt(api->curl.handle, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(api->curl.handle, CURLOPT_URL, curl);
    CURLcode res = curl_easy_perform(api->curl.handle);
    if(res != CURLE_OK) THROW("could not perform curl, error: %u", res);

clean:
    fclose(fp);
    free(curl);
    so_free(&subdirs);
    return err;
error:
    ERR_CLEAN;
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

