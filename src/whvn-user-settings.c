#include "whvn-user-settings.h"

void whvn_user_settings_free(WhvnUserSettings *settings) {
    ASSERT_ARG(settings);
    if(settings->user_blacklist) {
        vstr_free_set(&settings->user_blacklist);
        array_free(settings->user_blacklist);
    }
    if(settings->aspect_ratios) {
        vstr_free_set(&settings->aspect_ratios);
        array_free(settings->aspect_ratios);
    }
    if(settings->tag_blacklist) {
        vstr_free_set(&settings->tag_blacklist);
        array_free(settings->tag_blacklist);
    }
    if(settings->user_blacklist) {
        vstr_free_set(&settings->user_blacklist);
        array_free(settings->user_blacklist);
    }
}

