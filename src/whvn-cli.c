#include <rlarg.h>
#include "whvn-cli.h"
#include "whvn-api-search.h"
#include "whvn-purity.h"
#include <unistd.h>

static void wait_user(void) {
    printf(F("Press ENTER to continue ", IT FG_BK_B));
    getchar();
}

void whvn_cli_wallpaper_info_print(WhvnWallpaperInfo info, size_t index) {
    So color_s = SO;
    const char *purity_fmt = info.purity.nsfw ? FG_RD_B BOLD : info.purity.sketchy ? FG_YL_B BOLD : FG_GN_B BOLD;
    printf(F("%6zu", FG_BK_B) " " F("wallhaven-", IT) FS_BEG FS_FMT FS_END F("%.*s", IT) " " F("%.*s", UL FG_BL_B) " ",
            index, purity_fmt, SO_F(info.id), SO_F(so_get_ext(info.path)), SO_F(info.url));
    for(size_t j = 0; j < array_len(info.colors); ++j) {
        Color color = array_at(info.colors, j);
        so_clear(&color_s);
        if(color.rgba == 0x00) color.r = 0x01;
        So_Fx fx = { .bg = color };
        so_fmt_fx(&color_s, fx, 0, "  ");
        printf("%.*s", SO_F(color_s));
    }
    printf(" ");
    if(info.category.general) printf(F("G", BOLD FG_MG_B));
    if(info.category.anime) printf(F("A", BOLD FG_MG_B));
    if(info.category.people) printf(F("P", BOLD FG_MG_B));
    printf(" " F("󰣐 %lu", FG_RD_B) "", info.favorites);
    printf(" " F(" %lu", FG_CY_B) "", info.views);
    printf("\n");
    so_free(&color_s);
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

#include "whvn-api.h"
void whvn_cli_download(WhvnCli *cli, So folder, WhvnWallpaperInfo *info) {
    ASSERT_ARG(cli);
    ASSERT_ARG(info);
    So path = info->path;
    So basename = so_get_nodir(path);
    So filename = SO;
    so_path_join(&filename, cli->download_root, folder);
    so_path_join(&filename, filename, basename);
    printf("    " F(" ", FG_YL) " %.*s " F("-> %.*s", FG_BK_B IT) "", SO_F(basename), SO_F(folder));
    fflush(stdout);

    if(whvn_api_download(&cli->api, info, filename)) {
        printf("\r    " F(" ", FG_RD) " %.*s " F("-> %.*s", FG_BK_B IT) "\n", SO_F(path), SO_F(filename));
    } else {
        printf("\r    " F(" ", FG_GN) "\n");
    }
    //usleep(WHVN_API_RATE_US);
}

int whvn_cli_search(WhvnCli *cli) {
    ASSERT_ARG(cli);
    int result = 0;
    size_t n = 0;
    WhvnResponse response = {0};
    WhvnApiSearch search = cli->search;
    So out = {0};
    do {
        so_clear(&cli->api_buf);
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
                so_clear(&out);
                so_fmt(&out, "xdg-open \"%.*s\" 2>/dev/null &", SO_F(info.url));
                system(out.str);
            }
            if(cli->action.download) {
                whvn_cli_download(cli, so("search"), &info);
            }
        }
        usleep(WHVN_API_RATE_US);
        if(search.page >= response.meta.last_page) break;
        whvn_response_free(&response);
        search.page = search.page ? search.page + 1 : 2;
    } while(!result && (cli->max ? n < cli->max : true));
    so_free(&out);
    return result;
}

int whvn_cli_tag_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnTag tag_info = {0};
    int result = whvn_api_tag_info(&cli->api, cli->query.tag_info, &cli->api_buf, &tag_info);
    if(cli->action.print_pretty) {
        printf("%6lu %.*s (%.*s) - %zu \"%.*s\" %.*s - %.*s\n", tag_info.id, SO_F(tag_info.name), SO_F(tag_info.alias),
                tag_info.category_id, SO_F(tag_info.category), SO_F(whvn_purity_str(tag_info.purity)), SO_F(tag_info.created_at));
    }
    return result;
}

int whvn_cli_user_settings(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnUserSettings settings = {0};
    int result = whvn_api_user_settings(&cli->api, &cli->api_buf, &settings);
    if(cli->action.print_pretty) {
        printf("thumb_size: %.*s\n", SO_F(settings.thumb_size));
        printf("per_page: %zu\n", settings.per_page);
        printf("purity: %.*s\n", SO_F(whvn_purity_str(settings.purity)));
        printf("categories: %.*s\n", SO_F(whvn_category_str(settings.categories)));
        printf("resolutions:\n");
        for(size_t i = 0; i < array_len(settings.resolutions); ++i) {
            printf("  %.*s\n", SO_F(array_at(settings.resolutions, i)));
        }
        printf("aspect_ratios:\n");
        for(size_t i = 0; i < array_len(settings.aspect_ratios); ++i) {
            printf("  %.*s\n", SO_F(array_at(settings.aspect_ratios, i)));
        }
        printf("toplist_range: %.*s\n", SO_F(whvn_toplist_range_str(settings.toplist_range)));
        printf("tag_blacklist:\n");
        for(size_t i = 0; i < array_len(settings.tag_blacklist); ++i) {
            printf("  %.*s\n", SO_F(array_at(settings.tag_blacklist, i)));
        }
        printf("user_blacklist:\n");
        for(size_t i = 0; i < array_len(settings.user_blacklist); ++i) {
            printf("  %.*s\n", SO_F(array_at(settings.user_blacklist, i)));
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
            printf("%6zu %.*s %zux (%s)\n", collection.id, SO_F(collection.label), collection.count, collection.is_public ? "public" : "private");
        }
    }
    array_free(collections);
    return result;
}

int whvn_cli_user_collection(WhvnCli *cli) {
    ASSERT_ARG(cli);
    WhvnResponse response = {0};
    So username = {0};
    So base_download = SO;
    size_t id = 0, i = 0;
    bool valid_id = false;
    So id_name = SO;
    for(So splice = {0}; so_splice(cli->query.user_collection, &splice, '/'); ++i) {
        if(i > 1) THROW("invalid argument: %.*s", SO_F(cli->query.user_collection));
        else if(i == 0) username = splice;
        else if(i == 1) {
            if(so_as_size(splice, &id, 10)) {
                id_name = splice;
            } else {
                valid_id = true;
            }
        }
    }
    if(i < 2) THROW("missing ID");
    WhvnUserCollections collections = {0};
    int result = 0;
    result = whvn_api_user_collections(&cli->api, 0, &cli->api_buf, &collections);
    for(size_t i = 0; i < array_len(collections); ++i) {
        WhvnUserCollection collection = array_at(collections, i);
        if(!valid_id) {
            if(!so_cmp(collection.label, id_name)) {
                id = collection.id;
                valid_id = true;
                break;
            }
        } else {
            if(collection.id == id) {
                id_name = collection.label;
            }
        }
    }
    so_path_join(&base_download, username, id_name);
    //so_clear(&cli->api_buf);
    if(!valid_id) THROW("invalid id: %.*s", SO_F(id_name));
    WhvnApiSearch search = cli->search;
    size_t n = 0;
    So out = {0};
    do {
        so_clear(&cli->api_buf);
        result = whvn_api_user_collection(&cli->api, &search, username, id, &cli->api_buf, &response);
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
                so_clear(&out);
                so_fmt(&out, "xdg-open \"%.*s\" 2>/dev/null &", SO_F(info.url));
                system(out.str);
            }
            if(cli->action.download) {
                whvn_cli_download(cli, base_download, &info);
            }
        }
        usleep(WHVN_API_RATE_US);
        if(search.page >= response.meta.last_page) break;
        whvn_response_free(&response);
        search.page = search.page ? search.page + 1 : 2;
    } while(!result && (cli->max ? n < cli->max : true));
    so_free(&out);
    return result;
error:
    return -1;
}

int whvn_cli_check_apikey_present(WhvnCli *cli) {
    if(!so_len(cli->api.key)) {
        THROW("require API key");
    }
    return 0;
error:
    return -1;
}

int cli_ratio(WhvnCli *cli) {
    So recent = array_at(cli->vbuf_ratios, array_len(cli->vbuf_ratios) - 1);
    WhvnRatio ratio = whvn_ratio_parse(recent);
    if((ratio.h && ratio.w) || ratio.type != WHVN_RATIO) {
        array_push(cli->search.ratios, ratio);
        return 0;
    }
    THROW_PRINT("invalid ratio format: %.*s\n", SO_F(recent));
    return -1;
}

int cli_atleast(WhvnCli *cli) {
    So recent = cli->buf_atleast;
    WhvnResolution res = whvn_resolution_parse(recent);
    if(whvn_resolution_is_valid(res)) {
        cli->search.atleast = res;
        return 0;
    }
    THROW_PRINT("invalid resolution format: %.*s\n", SO_F(recent));
    return -1;
}

int cli_resolution(WhvnCli *cli) {
    So recent = array_at(cli->vbuf_resolutions, array_len(cli->vbuf_resolutions) - 1);
    WhvnResolution res = whvn_resolution_parse(recent);
    if(whvn_resolution_is_valid(res)) {
        array_push(cli->search.resolutions, res);
        return 0;
    }
    THROW_PRINT("invalid resolution format: %.*s\n", SO_F(recent));
    return -1;
}


int main(int argc, const char **argv) {
    int err = 0;

    WhvnCli cli = {0};
    WhvnCli def = {
        .api.url = so("https://wallhaven.cc/api/v1/"),
        .action.print_pretty = true,
        .max = 24,
    };
    so_extend_wordexp(&def.download_root, so("$HOME/Downloads/whvn"), false);
    cli.arg = arg_new();
    struct Arg *arg = cli.arg;
    arg_init(arg, so("whvn-cli"), so("wallhaven API cli"), so(F("https://github.com/rphii/c-whvn", FG_BL_B UL)));
    arg_init_width(cli.arg, 100, 45);
    arg_init_fmt(cli.arg);

    struct ArgX *x = 0;
    struct ArgXGroup *o = 0, *g = 0;
    o=argx_group(arg, so("Options"), false);
    argx_builtin_opt_help(o);
    argx_builtin_opt_source(o, so("/etc/whvn/whvn.conf"));
    argx_builtin_opt_source(o, so("$HOME/.config/rphiic/colors.conf"));
    argx_builtin_opt_source(o, so("$HOME/.config/whvn/whvn.conf"));
    argx_builtin_opt_source(o, so("$XDG_CONFIG_HOME/whvn/whvn.conf"));
    x=argx_init(o, 'U', so("url"), so("api URL"));
      argx_str(x, &cli.api.url, &def.api.url);
    x=argx_init(o, 'P', so("print"), so("print the raw API response"));
      g=argx_flag(x);
        x=argx_init(g, 0, so("url"), so("print the raw API URL"));
          argx_flag_set(x, &cli.api.print_url, 0);
        x=argx_init(g, 0, so("response"), so("print the raw API response"));
          argx_flag_set(x, &cli.api.print_response, 0);
    x=argx_init(o, 'n', so("max"), so("number of maximum results"));
      argx_ssz(x, &cli.max, &def.max);
    x=argx_init(o, 'a', so("action"), so("what to do with results"));
      g=argx_flag(x);
        x=argx_init(g, 0, so("pretty"), so(""));
          argx_flag_set(x, &cli.action.print_pretty, &def.action.print_pretty);
        x=argx_init(g, 0, so("browser"), so(""));
          argx_flag_set(x, &cli.action.open_browser, 0);
        x=argx_init(g, 0, so("wait"), so(""));
          argx_flag_set(x, &cli.action.wait_user, 0);
        x=argx_init(g, 0, so("download"), so(""));
          argx_flag_set(x, &cli.action.download, 0);
    x=argx_init(o, 'C', so("download-root"), so("output root directory for downloads"));
      argx_str(x, &cli.download_root, &def.download_root);
    x=argx_pos(arg, so("api-call"), so("select api call"));
      g=argx_opt(x, 0, 0);
        x=argx_init(g, 0, so("wallpaper-info"), so("get wallpaper info"));
          argx_str(x, &cli.query.wallpaper_info, 0);
          argx_func(x, 2, whvn_cli_wallpaper_info, &cli, false, false);
        x=argx_init(g, 0, so("search"), so("search wallpapers\n"
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
        x=argx_init(g, 0, so("tag-info"), so("get wallpaper tag info"));
          argx_str(x, &cli.query.tag_info, 0);
          argx_func(x, 2, whvn_cli_tag_info, &cli, false, false);
        x=argx_init(g, 0, so("user-settings"), so("get user settings"));
          argx_func(x, 1, whvn_cli_user_settings, &cli, false, false);
        x=argx_init(g, 0, so("user-collections"), so("get user collections"));
          argx_func(x, 2, whvn_cli_user_collections, &cli, false, false);
        x=argx_init(g, 0, so("user-collection"), so("get a user's collection"));
          argx_str(x, &cli.query.user_collection, 0);
          argx_func(x, 2, whvn_cli_user_collection, &cli, false, false);
          argx_type(x, so("user/id"));
    x=argx_init(o, 'c', so("categories"), so("search: categories"));
      g=argx_flag(x);
        x=argx_init(g, 0, so("general"), so(""));
          argx_flag_set(x, &cli.search.categories.general, 0);
        x=argx_init(g, 0, so("anime"), so(""));
          argx_flag_set(x, &cli.search.categories.anime, 0);
        x=argx_init(g, 0, so("people"), so(""));
          argx_flag_set(x, &cli.search.categories.people, 0);
    x=argx_init(o, 'p', so("purity"), so("search: purity"));
      g=argx_flag(x);
        x=argx_init(g, 0, so("sfw"), so(""));
          argx_flag_set(x, &cli.search.purity.sfw, 0);
        x=argx_init(g, 0, so("sketchy"), so(""));
          argx_flag_set(x, &cli.search.purity.sketchy, 0);
        x=argx_init(g, 0, so("nsfw"), so("requires API key"));
          argx_flag_set(x, &cli.search.purity.nsfw, 0);
          argx_func(x, 1, whvn_cli_check_apikey_present, &cli, false, false);
    x=argx_init(o, 's', so("sorting"), so("search: sorting"));
      g=argx_opt(x, (int *)&cli.search.sorting, 0);
        x=argx_init(g, 0, so("date_added"), so(""));
          argx_opt_enum(x, WHVN_SORTING_DATE_ADDED);
        x=argx_init(g, 0, so("relevance"), so(""));
          argx_opt_enum(x, WHVN_SORTING_RELEVANCE);
        x=argx_init(g, 0, so("random"), so(""));
          argx_opt_enum(x, WHVN_SORTING_RANDOM);
        x=argx_init(g, 0, so("views"), so(""));
          argx_opt_enum(x, WHVN_SORTING_VIEWS);
        x=argx_init(g, 0, so("favorites"), so(""));
          argx_opt_enum(x, WHVN_SORTING_FAVORITES);
        x=argx_init(g, 0, so("toplist"), so(""));
          argx_opt_enum(x, WHVN_SORTING_TOPLIST);
    x=argx_init(o, 'o', so("order"), so("search: order"));
      g=argx_opt(x, (int *)&cli.search.order, 0);
        x=argx_init(g, 0, so("asc"), so(""));
          argx_opt_enum(x, WHVN_ORDER_ASC);
        x=argx_init(g, 0, so("desc"), so(""));
          argx_opt_enum(x, WHVN_ORDER_DESC);
    x=argx_init(o, 't', so("toplist-range"), so("search: toplist range"));
      g=argx_opt(x, (int *)&cli.search.toplist_range, 0);
        x=argx_init(g, 0, so("1d"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1D);
        x=argx_init(g, 0, so("3d"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_3D);
        x=argx_init(g, 0, so("1w"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1W);
        x=argx_init(g, 0, so("1M"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1M);
        x=argx_init(g, 0, so("3M"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_3M);
        x=argx_init(g, 0, so("6M"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_6M);
        x=argx_init(g, 0, so("1y"), so(""));
          argx_opt_enum(x, WHVN_TOPLIST_RANGE_1Y);
    x=argx_init(o, 'i', so("page"), so("search: page"));
      argx_int(x, (int *)&cli.search.page, 0);
    x=argx_init(o, 'r', so("seed"), so("search: seed"));
      argx_str(x, &cli.search.seed, 0);

    x=argx_init(o, 'R', so("ratios"), so("search: ratios\n"
                "e.g.: '1920x1080', 'landscape' or 'portrait'"));
      argx_vstr(x, &cli.vbuf_ratios, 0);
      argx_type(x, so("ratios"));
      argx_func(x, 0, &cli_ratio, &cli, false, false);

    x=argx_init(o, 'A', so("atleast"), so("search: atleast\n"
                "e.g.: '1920x1080'"));
      argx_str(x, &cli.buf_atleast, 0);
      argx_type(x, so("resolution"));
      argx_func(x, 0, &cli_atleast, &cli, false, false);

    x=argx_init(o, 'Z', so("resolutions"), so("search: resolutions\n"
                "e.g.: '1920x1080'"));
      argx_vstr(x, &cli.vbuf_resolutions, 0);
      argx_type(x, so("resolutions"));
      argx_func(x, 0, &cli_resolution, &cli, false, false);

#if 0
    x=argx_init(o, 'C', so("color"), so("search: color"));
      g=argx_opt(x, 0, 0);
        x=argx_opt_enum(g, 0, 
F( "660000", FG3(0x66, 0x00, 0x00))
F( "990000", FG3(0x99, 0x00, 0x00))
F( "cc0000", FG3(0xcc, 0x00, 0x00))
F( "cc3333", FG3(0xcc, 0x33, 0x33))
F( "ea4c88", FG3(0xea, 0x4c, 0x88))
F( "993399", FG3(0x99, 0x33, 0x99))
F( "663399", FG3(0x66, 0x33, 0x99))
F( "333399", FG3(0x33, 0x33, 0x99))
F( "0066cc", FG3(0x00, 0x66, 0xcc))
F( "0099cc", FG3(0x00, 0x99, 0xcc))
F( "66cccc", FG3(0x66, 0xcc, 0xcc))
F( "77cc33", FG3(0x77, 0xcc, 0x33))
F( "669900", FG3(0x66, 0x99, 0x00))
F( "336600", FG3(0x33, 0x66, 0x00))
F( "666600", FG3(0x66, 0x66, 0x00))
F( "999900", FG3(0x99, 0x99, 0x00))
F( "cccc33", FG3(0xcc, 0xcc, 0x33))
F( "ffff00", FG3(0xff, 0xff, 0x00))
F( "ffcc33", FG3(0xff, 0xcc, 0x33))
F( "ff9900", FG3(0xff, 0x99, 0x00))
F( "ff6600", FG3(0xff, 0x66, 0x00))
F( "cc6633", FG3(0xcc, 0x66, 0x33))
F( "996633", FG3(0x99, 0x66, 0x33))
F( "663300", FG3(0x66, 0x33, 0x00))
F( "000000", FG3(0x00, 0x00, 0x00))
F( "999999", FG3(0x99, 0x99, 0x99))
F( "cccccc", FG3(0xcc, 0xcc, 0xcc))
F( "ffffff", FG3(0xff, 0xff, 0xff))
F( "424153", FG3(0x42, 0x41, 0x53))
#endif

    o=argx_group(arg, so("Environment Variables"), false);
    x=argx_env(o, so("WHVN_API_KEY"), so("your API key"), true);
    argx_builtin_env_compgen(o);
      argx_str(x, &cli.api.key, 0);

    o=argx_group(arg, so("Color Adjustments"), true);
    argx_builtin_opt_rice(o);
 

    bool quit_early = false;
    TRYC(arg_parse(arg, argc, argv, &quit_early));

clean:
    whvn_api_search_free(&cli.search);
    whvn_api_free(&cli.api);
    so_free(&cli.api_buf);
    arg_free(&cli.arg);
    //printf("done\n");
    return err;
error:
    ERR_CLEAN;
}

