#include <curl/curl.h>
#include <rphii/arg.h>
#include "whvn-cli.h"
#include "whvn-collection.h"
#include "whvn-api.h"

int main(int argc, const char **argv) {
    int err = 0;
    WhvnCli w = {0};

#if 0
    whvn_collection_init(&coll);
    char tmp[L_tmpnam];
    for(int i = 0; i < 10000; ++i) {
        if(!((i+1)%2)) {
            tmpnam(tmp);
        }
        WhvnWallpaperInfo w1 = { .id = str_l(tmp) };
        whvn_collection_add(&coll, &w1);
    }
    whvn_collection_free(&coll);
#endif

    WhvnCollection coll = {0};
    WhvnApi api = {0};
    WhvnWallpaperInfo info = {0};
    whvn_api_wallpaper_info(&api, STR("vpmexp"), &info);
    whvn_collection_add(&coll, &api.wallpaper_info);

    whvn_api_free(&api);
    whvn_collection_free(&coll);

    whvn_arg_init(&w.arg);

    bool quit_early = false;
    try(whvn_arg_parse(w.arg, argc, argv, &quit_early));

clean:
    whvn_arg_free(&w.arg);
    return err;
error:
    ERR_CLEAN;
}

