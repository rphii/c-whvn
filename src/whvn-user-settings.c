#include "whvn-user-settings.h"

void whvn_user_settings_free(WhvnUserSettings *settings) {
    ASSERT_ARG(settings);
    if(settings->user_blacklist) {
        vso_free(&settings->user_blacklist);
    }
    if(settings->aspect_ratios) {
        vso_free(&settings->aspect_ratios);
    }
    if(settings->tag_blacklist) {
        vso_free(&settings->tag_blacklist);
    }
    if(settings->user_blacklist) {
        vso_free(&settings->user_blacklist);
    }
}

