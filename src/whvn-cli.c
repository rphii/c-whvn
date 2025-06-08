#include <rphii/arg.h>
#include "whvn-cli.h"

int whvn_cli_wallpaper_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnWallpaperInfo info = {0};
    int result = whvn_api_wallpaper_info(&cli->api, cli->query.wallpaper_info, &info);
    if(cli->print_pretty) {
        Str color_s = STR_DYN();
        printf(F("%6s", FG_BK_B) " " F("wallhaven-", IT) F("%.*s", BOLD) F("%.*s", IT) " " F("%.*s", UL FG_BL) " ", "", STR_F(info.id), STR_F(str_get_ext(info.path)), STR_F(info.url));
        for(size_t j = 0; j < array_len(info.colors); ++j) {
            Color color = array_at(info.colors, j);
            str_clear(&color_s);
            if(color.rgba == 0x00) color.r = 0x01;
            color_fmt_rgb_fmt(&color_s, color, str("  "));
            printf("%.*s", STR_F(color_s));
        }
        printf(" " F("󰣐 %lu", FG_RD_B) "", info.favorites);
        printf("\n");
    }
    return result;
}
int whvn_cli_search(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnResponse response = {0};
    int result = whvn_api_search(&cli->api, &cli->search, &response);
    if(cli->print_pretty) {
        Str color_s = STR_DYN();
        for(size_t i = 0; i < array_len(response.data); ++i) {
            WhvnWallpaperInfo info = array_at(response.data, i);
            printf(F("%6zu", FG_BK_B) " " F("wallhaven-", IT) F("%.*s", BOLD) F("%.*s", IT) " " F("%.*s", UL FG_BL) " ", i, STR_F(info.id), STR_F(str_get_ext(info.path)), STR_F(info.url));
            for(size_t j = 0; j < array_len(info.colors); ++j) {
                Color color = array_at(info.colors, j);
                str_clear(&color_s);
                if(color.rgba == 0x00) color.r = 0x01;
                color_fmt_rgb_fmt(&color_s, color, str("  "));
                printf("%.*s", STR_F(color_s));
            }
            printf(" " F("󰣐 %lu", FG_RD_B) "", info.favorites);
            printf("\n");
        }
    }
    return result;
}
int whvn_cli_tag_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnTag tag_info = {0};
    int result = whvn_api_tag_info(&cli->api, cli->query.tag_info, &tag_info);
    if(cli->print_pretty) {
        printf("%6lu %.*s (%.*s) - %zu \"%.*s\" %.*s - %.*s\n", tag_info.id, STR_F(tag_info.name), STR_F(tag_info.alias),
                tag_info.category_id, STR_F(tag_info.category), STR_F(whvn_purity_str(tag_info.purity)), STR_F(tag_info.created_at));
    }
    return result;
}
int whvn_cli_user_settings(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnUserSettings settings = {0};
    int result = whvn_api_user_settings(&cli->api, str(""), &settings);
    if(cli->print_pretty) {
        printf("thumb_size: %.*s\n", STR_F(settings.thumb_size));
        printf("per_page: %zu\n", settings.per_page);
        printf("purity: %.*s\n", STR_F(whvn_purity_str(settings.purity)));
        printf("categories: %.*s\n", STR_F(whvn_category_str(settings.categories)));
        printf("resolutions:\n");
        for(size_t i = 0; i < array_len(settings.resolutions); ++i) {
            printf("  %.*s\n", STR_F(array_at(settings.resolutions, i)));
        }
        printf("aspect_ratios:\n");
        for(size_t i = 0; i < array_len(settings.aspect_ratios); ++i) {
            printf("  %.*s\n", STR_F(array_at(settings.aspect_ratios, i)));
        }
        printf("toplist_range: %.*s\n", STR_F(whvn_toplist_range_str(settings.toplist_range)));
        printf("tag_blacklist:\n");
        for(size_t i = 0; i < array_len(settings.tag_blacklist); ++i) {
            printf("  %.*s\n", STR_F(array_at(settings.tag_blacklist, i)));
        }
        printf("user_blacklist:\n");
        for(size_t i = 0; i < array_len(settings.user_blacklist); ++i) {
            printf("  %.*s\n", STR_F(array_at(settings.user_blacklist, i)));
        }
        printf("ai_art_filter: %s\n", settings.ai_art_filter ? "true" : "false");
    }
    return result;
}
int whvn_cli_user_collections(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnUserCollections collections = {0};
    int result = whvn_api_user_collections(&cli->api, cli->query.user_collection, &collections);
    if(cli->print_pretty) {
        for(size_t i = 0; i < array_len(collections); ++i) {
            WhvnUserCollection collection = array_at(collections, i);
            printf("%6zu %.*s %zux (%s)\n", collection.id, STR_F(collection.label), collection.count, collection.is_public ? "public" : "private");
        }
    }
    return result;
}
int whvn_cli_user_collection(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnResponse response = {0};
    int result = whvn_api_user_collection(&cli->api, cli->query.user_collection, &response);
    if(cli->print_pretty) {
        Str color_s = STR_DYN();
        for(size_t i = 0; i < array_len(response.data); ++i) {
            WhvnWallpaperInfo info = array_at(response.data, i);
            printf(F("%6zu", FG_BK_B) " " F("wallhaven-", IT) F("%.*s", BOLD) F("%.*s", IT) " " F("%.*s", UL FG_BL) " ", i, STR_F(info.id), STR_F(str_get_ext(info.path)), STR_F(info.url));
            for(size_t j = 0; j < array_len(info.colors); ++j) {
                Color color = array_at(info.colors, j);
                str_clear(&color_s);
                if(color.rgba == 0x00) color.r = 0x01;
                color_fmt_rgb_fmt(&color_s, color, str("  "));
                printf("%.*s", STR_F(color_s));
            }
            printf(" " F("󰣐 %lu", FG_RD_B) "", info.favorites);
            printf("\n");
        }
    }
    return result;
}

int main(int argc, const char **argv) {
    int err = 0;

    WhvnCli cli = {0};
    cli.arg = arg_new();
    struct Arg *arg = cli.arg;
    arg_init(arg, str("whvn-cli"), str("wallhaven API cli"), str("https://github.com/rphii/c-whvn"));
    arg_init_width(cli.arg, 0, 90);

    struct ArgX *x = 0;
    struct ArgXGroup *g = 0;
    x=argx_init(arg_opt(arg), 'h', str("help"), str("display this help"));
      argx_help(x, arg);
    x=argx_init(arg_opt(arg), 'p', str("print"), str("print the raw API response"));
      g=argx_flag(x);
        x=argx_init(g, 0, str("url"), str("print the raw API URL"));
          argx_flag_set(x, &cli.api.print_url, 0);
        x=argx_init(g, 0, str("response"), str("print the raw API response"));
          argx_flag_set(x, &cli.api.print_response, 0);
        x=argx_init(g, 0, str("pretty"), str("print the pretty output"));
          argx_flag_set(x, &cli.print_pretty, 0);
    x=argx_pos(arg, str("api-call"), str("select api call"));
      g=argx_opt(x, 0, 0);
        x=argx_init(g, 0, str("wallpaper-info"), str("get wallpaper info"));
          argx_str(x, &cli.query.wallpaper_info, 0);
          argx_func(x, 2, whvn_cli_wallpaper_info, &cli, false);
        x=argx_init(g, 0, str("search"), str("search wallpapers"));
          argx_str(x, &cli.search.query, 0);
          argx_func(x, 2, whvn_cli_search, &cli, false);
        x=argx_init(g, 0, str("tag-info"), str("get wallpaper tag info"));
          argx_str(x, &cli.query.tag_info, 0);
          argx_func(x, 2, whvn_cli_tag_info, &cli, false);
        x=argx_init(g, 0, str("user-settings"), str("get user settings"));
          argx_func(x, 1, whvn_cli_user_settings, &cli, false);
        x=argx_init(g, 0, str("user-collections"), str("get user collections"));
          argx_func(x, 2, whvn_cli_user_collections, &cli, false);
        x=argx_init(g, 0, str("user-collection"), str("get a user's collection"));
          argx_str(x, &cli.query.user_collection, 0);
          argx_func(x, 2, whvn_cli_user_collection, &cli, false);
          argx_type(x, str("user/id"));

    //whvn_api_wallpaper_info(&cli.api, STR("vpmexp"), &cli.info);
    //whvn_collection_add(&cli.coll, &cli.info);

    argx_env(cli.arg, str("WHVN_API_KEY"), str("your API key"), &cli.api.key, 0, true);

    bool quit_early = false;
    TRYC(arg_parse(arg, argc, argv, &quit_early));

clean:
    whvn_api_free(&cli.api);
    arg_free(&cli.arg);
    //printf("done\n");
    return err;
error:
    ERR_CLEAN;
}

