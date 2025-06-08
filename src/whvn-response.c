#include "whvn-response.h"

void whvn_response_free(WhvnResponse *response) {
    if(!response) return;
    for(size_t i = 0; i < array_len(response->data); ++i) {
        WhvnWallpaperInfo info = array_at(response->data, i);
        whvn_wallpaper_info_free(&info);
    }
    array_free(response->data);
    str_free(&response->meta.query);
    str_free(response->meta.seed);
#ifndef NDEBUG
    memset(response, 0, sizeof(*response));
#endif
}

