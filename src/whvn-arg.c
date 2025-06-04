#include "whvn-arg.h"

void whvn_arg_init(struct Arg **arg_p) {
    *arg_p = arg_new();
    struct Arg *arg = *arg_p;
    arg_init(arg, str("whvn-cli"), str("wallhaven API cli"), str("https://github.com/rphii/c-whvn"));

    struct ArgX *x = 0;
    struct ArgXGroup *g = 0;
    x=argx_init(arg_opt(arg), 'h', str("help"), str("display this help"));
      argx_help(x, arg);
    x=argx_pos(arg, str("wallpaper-info"), str("get wallpaper info"));
    x=argx_pos(arg, str("search"), str("search wallpapers"));
    x=argx_pos(arg, str("tag-info"), str("get wallpaper tag info"));
    x=argx_pos(arg, str("user-settings"), str("get user settings"));
    x=argx_pos(arg, str("user-collections"), str("get user collections"));

    //ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, WhvnWallpaperInfo *info);
    //ErrDecl whvn_api_search(WhvnApi *api, Str arg, WhvnCollection *collection);
    //ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, WhvnTags *tags);
    //ErrDecl whvn_api_user_settings(WhvnApi *api, Str arg, WhvnUserSettings *settings);
    //ErrDecl whvn_api_user_collections(WhvnApi *api, Str arg, WhvnUserCollections *collections);
    //x=argx_init(arg_opt(arg), '

}

ErrDecl whvn_arg_parse(struct Arg *arg, const int argc, const char **argv, bool *quit_early) {
    TRYC(arg_parse(arg, argc, argv, quit_early));
    return 0;
error:
    return -1;
}

void whvn_arg_free(struct Arg **arg_p) {
    arg_free(arg_p);
}

