#include <rphii/arg.h>
#include "whvn-cli.h"
#include "whvn-api-search.h"
#include "whvn-purity.h"
#include <unistd.h>

static void wait_user(void) {
    printf(F("Press ENTER to continue ", IT FG_BK_B));
    getchar();
}

void whvn_cli_wallpaper_info_print(WhvnWallpaperInfo info, size_t index) {
    Str color_s = STR_DYN();
    const char *purity_fmt = info.purity.nsfw ? FG_RD_B BOLD : info.purity.sketchy ? FG_YL_B BOLD : FG_GN_B BOLD;
    printf(F("%6zu", FG_BK_B) " " F("wallhaven-", IT) FS_BEG FS_FMT FS_END F("%.*s", IT) " " F("%.*s", UL FG_BL_B) " ",
            index, purity_fmt, STR_F(info.id), STR_F(str_get_ext(info.path)), STR_F(info.url));
    for(size_t j = 0; j < array_len(info.colors); ++j) {
        Color color = array_at(info.colors, j);
        str_clear(&color_s);
        if(color.rgba == 0x00) color.r = 0x01;
        color_fmt_rgb_fmt(&color_s, color, str("  "));
        printf("%.*s", STR_F(color_s));
    }
    printf(" ");
    if(info.category.general) printf(F("G", BOLD FG_MG_B));
    if(info.category.anime) printf(F("A", BOLD FG_MG_B));
    if(info.category.people) printf(F("P", BOLD FG_MG_B));
    printf(" " F("󰣐 %lu", FG_RD_B) "", info.favorites);
    printf(" " F(" %lu", FG_CY_B) "", info.views);
    printf("\n");
    str_free(&color_s);
}

int whvn_cli_wallpaper_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnWallpaperInfo info = {0};
    int result = whvn_api_wallpaper_info(&cli->api, cli->query.wallpaper_info, &cli->api_buf, &info);
    if(cli->action.print_pretty) {
        whvn_cli_wallpaper_info_print(info, 0);
    }
    whvn_wallpaper_info_free(&info);
    return result;
}

int whvn_cli_search(WhvnCli *cli) {
    ASSERT_ARG(cli);
    int result = 0;
    size_t n = 0;
    WhvnResponse response = {0};
    WhvnApiSearch search = cli->search;
    Str out = {0};
    do {
        str_clear(&cli->api_buf);
        result = whvn_api_search(&cli->api, &search, &cli->api_buf, &response);
        for(size_t i = 0; !result && i < array_len(response.data); ++i, ++n) {
            WhvnWallpaperInfo info = array_at(response.data, i);
            if(cli->max && n >= cli->max) break;
            if(cli->action.open_browser) {
                usleep(WHVN_API_RATE_US);
                if(i && cli->action.wait_user) wait_user();
            }
            if(cli->action.print_pretty) {
                whvn_cli_wallpaper_info_print(info, n);
            }
            if(cli->action.open_browser) {
                str_clear(&out);
                str_fmt(&out, "xdg-open \"%.*s\" 2>/dev/null &", STR_F(info.url));
                system(out.str);
            }
        }
        whvn_response_free(&response);
        usleep(WHVN_API_RATE_US);
        search.page = search.page ? search.page + 1 : 2;
    } while(!result && (cli->max ? n < cli->max : false));
    str_free(&out);
    return result;
}

int whvn_cli_tag_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnTag tag_info = {0};
    int result = whvn_api_tag_info(&cli->api, cli->query.tag_info, &cli->api_buf, &tag_info);
    if(cli->action.print_pretty) {
        printf("%6lu %.*s (%.*s) - %zu \"%.*s\" %.*s - %.*s\n", tag_info.id, STR_F(tag_info.name), STR_F(tag_info.alias),
                tag_info.category_id, STR_F(tag_info.category), STR_F(whvn_purity_str(tag_info.purity)), STR_F(tag_info.created_at));
    }
    return result;
}

int whvn_cli_user_settings(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnUserSettings settings = {0};
    int result = whvn_api_user_settings(&cli->api, &cli->api_buf, &settings);
    if(cli->action.print_pretty) {
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
    whvn_user_settings_free(&settings);
    return result;
}

int whvn_cli_user_collections(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnUserCollections collections = {0};
    int result = whvn_api_user_collections(&cli->api, cli->search.page, &cli->api_buf, &collections);
    if(cli->action.print_pretty) {
        for(size_t i = 0; i < array_len(collections); ++i) {
            WhvnUserCollection collection = array_at(collections, i);
            printf("%6zu %.*s %zux (%s)\n", collection.id, STR_F(collection.label), collection.count, collection.is_public ? "public" : "private");
        }
    }
    array_free(collections);
    return result;
}

int whvn_cli_user_collection(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnResponse response = {0};
    Str username = {0};
    size_t id = 0, i = 0;
    for(Str splice = {0}; str_splice(cli->query.user_collection, &splice, '/'); ++i) {
        if(i > 1) THROW("invalid argument: %.*s", STR_F(cli->query.user_collection));
        else if(i == 0) username = splice;
        else if(i == 1) if(str_as_size(splice, &id, 10)) THROW("invalid ID: %.*s", STR_F(splice));
    }
    if(i < 2) THROW("missing ID");
    int result = 0;
    WhvnApiSearch search = cli->search;
    size_t n = 0;
    Str out = {0};
    do {
        str_clear(&cli->api_buf);
        result = whvn_api_user_collection(&cli->api, search.page, username, id, &cli->api_buf, &response);
        for(size_t i = 0; !result && i < array_len(response.data); ++i, ++n) {
            WhvnWallpaperInfo info = array_at(response.data, i);
            if(cli->max && n >= cli->max) break;
            if(cli->action.open_browser) {
                usleep(WHVN_API_RATE_US);
                if(i && cli->action.wait_user) wait_user();
            }
            if(cli->action.print_pretty) {
                whvn_cli_wallpaper_info_print(info, n);
            }
            if(cli->action.open_browser) {
                str_clear(&out);
                str_fmt(&out, "xdg-open \"%.*s\" 2>/dev/null &", STR_F(info.url));
                system(out.str);
            }
        }
        whvn_response_free(&response);
        usleep(WHVN_API_RATE_US);
        search.page = search.page ? search.page + 1 : 2;
    } while(!result && (cli->max ? n < cli->max : false));
    str_free(&out);
    return result;
error:
    return -1;
}

int whvn_cli_check_apikey_present(WhvnCli *cli) {
    if(!str_len_raw(cli->api.key)) {
        THROW("require API key");
    }
    return 0;
error:
    return -1;
}

int main(int argc, const char **argv) {
    int err = 0;

    WhvnCli cli = {0};
    WhvnCli def = {
        .api.url = str("https://wallhaven.cc/api/v1/"),
        .action.print_pretty = true,
    };
    cli.arg = arg_new();
    struct Arg *arg = cli.arg;
    arg_init(arg, str("whvn-cli"), str("wallhaven API cli"), str(F("https://github.com/rphii/c-whvn", FG_BL_B UL)));
    arg_init_width(cli.arg, 100, 45);
    arg_init_fmt(cli.arg);

    struct ArgX *x = 0;
    struct ArgXGroup *o = 0, *g = 0;
    o=argx_group(arg, str("Options"), false);
    argx_builtin_opt_help(o);
    argx_builtin_opt_source(o, str("/etc/whvn/whvn.conf"));
    argx_builtin_opt_source(o, str("$HOME/.config/rphiic/colors.conf"));
    argx_builtin_opt_source(o, str("$HOME/.config/whvn/whvn.conf"));
    argx_builtin_opt_source(o, str("$XDG_CONFIG_HOME/whvn/whvn.conf"));
    x=argx_init(o, 'U', str("url"), str("api URL"));
      argx_str(x, &cli.api.url, &def.api.url);
    x=argx_init(o, 'P', str("print"), str("print the raw API response"));
      g=argx_flag(x);
        x=argx_init(g, 0, str("url"), str("print the raw API URL"));
          argx_flag_set(x, &cli.api.print_url, 0);
        x=argx_init(g, 0, str("response"), str("print the raw API response"));
          argx_flag_set(x, &cli.api.print_response, 0);
    x=argx_init(o, 'n', str("max"), str("number of maximum results"));
      argx_ssz(x, &cli.max, 0);
    x=argx_init(o, 'a', str("action"), str("what to do with results"));
      g=argx_flag(x);
        x=argx_init(g, 0, str("pretty"), str(""));
          argx_flag_set(x, &cli.action.print_pretty, &def.action.print_pretty);
        x=argx_init(g, 0, str("browser"), str(""));
          argx_flag_set(x, &cli.action.open_browser, 0);
        x=argx_init(g, 0, str("wait"), str(""));
          argx_flag_set(x, &cli.action.wait_user, 0);
    x=argx_pos(arg, str("api-call"), str("select api call"));
      g=argx_opt(x, 0, 0);
        x=argx_init(g, 0, str("wallpaper-info"), str("get wallpaper info"));
          argx_str(x, &cli.query.wallpaper_info, 0);
          argx_func(x, 2, whvn_cli_wallpaper_info, &cli, false, false);
        x=argx_init(g, 0, str("search"), str("search wallpapers\n"
            "tagname - search fuzzily for a tag/keyword\n"
            "-tagname - exclude a tag/keyword\n"
            "+tag1 +tag2 - must have tag1 and tag2\n"
            "+tag1 -tag2 - must have tag1 and NOT tag2\n"
            "@username - user uploads\n"
            "id:123 - Exact tag search (can not be combined)\n"
            "type:{png/jpg} - Search for file type (jpg = jpeg)\n"
            "like:wallpaper ID - Find wallpapers with similar tags"));
          argx_str(x, &cli.search.query, 0);
          argx_func(x, 2, whvn_cli_search, &cli, false, false);
        x=argx_init(g, 0, str("tag-info"), str("get wallpaper tag info"));
          argx_str(x, &cli.query.tag_info, 0);
          argx_func(x, 2, whvn_cli_tag_info, &cli, false, false);
        x=argx_init(g, 0, str("user-settings"), str("get user settings"));
          argx_func(x, 1, whvn_cli_user_settings, &cli, false, false);
        x=argx_init(g, 0, str("user-collections"), str("get user collections"));
          argx_func(x, 2, whvn_cli_user_collections, &cli, false, false);
        x=argx_init(g, 0, str("user-collection"), str("get a user's collection"));
          argx_str(x, &cli.query.user_collection, 0);
          argx_func(x, 2, whvn_cli_user_collection, &cli, false, false);
          argx_type(x, str("user/id"));
    x=argx_init(o, 'c', str("categories"), str("search: categories"));
      g=argx_flag(x);
        x=argx_init(g, 0, str("general"), str(""));
          argx_flag_set(x, &cli.search.categories.general, 0);
        x=argx_init(g, 0, str("anime"), str(""));
          argx_flag_set(x, &cli.search.categories.anime, 0);
        x=argx_init(g, 0, str("people"), str(""));
          argx_flag_set(x, &cli.search.categories.people, 0);
    x=argx_init(o, 'p', str("purity"), str("search: purity"));
      g=argx_flag(x);
        x=argx_init(g, 0, str("sfw"), str(""));
          argx_flag_set(x, &cli.search.purity.sfw, 0);
        x=argx_init(g, 0, str("sketchy"), str(""));
          argx_flag_set(x, &cli.search.purity.sketchy, 0);
        x=argx_init(g, 0, str("nsfw"), str("requires API key"));
          argx_flag_set(x, &cli.search.purity.nsfw, 0);
          argx_func(x, 1, whvn_cli_check_apikey_present, &cli, false, false);
    x=argx_init(o, 's', str("sorting"), str("search: sorting"));
      g=argx_opt(x, (int *)&cli.search.sorting, 0);
        x=argx_init(g, 0, str("date_added"), str(""));
          argx_opt_enum(x, WHVN_SORTING_DATE_ADDED);
        x=argx_init(g, 0, str("relevance"), str(""));
          argx_opt_enum(x, WHVN_SORTING_RELEVANCE);
        x=argx_init(g, 0, str("random"), str(""));
          argx_opt_enum(x, WHVN_SORTING_RANDOM);
        x=argx_init(g, 0, str("views"), str(""));
          argx_opt_enum(x, WHVN_SORTING_VIEWS);
        x=argx_init(g, 0, str("favorites"), str(""));
          argx_opt_enum(x, WHVN_SORTING_FAVORITES);
        x=argx_init(g, 0, str("toplist"), str(""));
          argx_opt_enum(x, WHVN_SORTING_TOPLIST);
    x=argx_init(o, 'o', str("order"), str("search: order"));
      g=argx_opt(x, (int *)&cli.search.order, 0);
        x=argx_init(g, 0, str("asc"), str(""));
          argx_opt_enum(x, WHVN_ORDER_ASC);
        x=argx_init(g, 0, str("desc"), str(""));
          argx_opt_enum(x, WHVN_ORDER_DESC);
    x=argx_init(o, 't', str("toplist-range"), str("search: toplist range"));
      g=argx_opt(x, (int *)&cli.search.toplist_range, 0);
        x=argx_init(g, 0, str("1d"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1D);
        x=argx_init(g, 0, str("3d"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_3D);
        x=argx_init(g, 0, str("1w"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1W);
        x=argx_init(g, 0, str("1M"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1M);
        x=argx_init(g, 0, str("3M"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_3M);
        x=argx_init(g, 0, str("6M"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_6M);
        x=argx_init(g, 0, str("1y"), str(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1Y);
    x=argx_init(o, 'i', str("page"), str("search: page"));
      argx_int(x, (int *)&cli.search.page, 0);
    x=argx_init(o, 'r', str("seed"), str("search: seed"));
      argx_str(x, &cli.search.seed, 0);

    o=argx_group(arg, str("Environment Variables"), false);
    x=argx_env(o, str("WHVN_API_KEY"), str("your API key"), true);
    argx_builtin_env_compgen(o);
      argx_str(x, &cli.api.key, 0);

    o=argx_group(arg, str("Color Adjustments"), true);
    argx_builtin_opt_rice(o);
 

    bool quit_early = false;
    TRYC(arg_parse(arg, argc, argv, &quit_early));

clean:
    whvn_api_free(&cli.api);
    str_free(&cli.api_buf);
    arg_free(&cli.arg);
    //printf("done\n");
    return err;
error:
    ERR_CLEAN;
}

