#include <rphii/arg.h>
#include "whvn-cli.h"

int whvn_cli_wallpaper_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    return whvn_api_wallpaper_info(&cli->api, cli->query.wallpaper_info, &cli->result.wallpaper_info);
}
int whvn_cli_search(WhvnCli *cli) {
    ASSERT_ARG(cli);
    return whvn_api_search(&cli->api, cli->query.search, &cli->result.search);
}
int whvn_cli_tag_info(WhvnCli *cli) {
    ASSERT_ARG(cli);
    return whvn_api_tag_info(&cli->api, cli->query.tag_info, &cli->result.tag_info);
}
int whvn_cli_user_settings(WhvnCli *cli) {
    ASSERT_ARG(cli);
    return whvn_api_user_settings(&cli->api, str(""), &cli->result.user_settings);
}
int whvn_cli_user_collections(WhvnCli *cli) {
    ASSERT_ARG(cli);
    return whvn_api_user_collections(&cli->api, cli->query.user_collection, &cli->result.user_collections);
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
    x=argx_pos(arg, str("api-call"), str("select api call"));
      g=argx_opt(x, 0, 0);
        x=argx_init(g, 0, str("wallpaper-info"), str("get wallpaper info"));
          argx_str(x, &cli.query.wallpaper_info, 0);
          argx_func(x, 2, whvn_cli_wallpaper_info, &cli, false);
        x=argx_init(g, 0, str("search"), str("search wallpapers"));
          argx_str(x, &cli.query.search, 0);
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
          argx_func(x, 2, whvn_cli_user_collections, &cli, false);
          argx_type(x, str("user/id"));

    //whvn_api_wallpaper_info(&cli.api, STR("vpmexp"), &cli.info);
    //whvn_collection_add(&cli.coll, &cli.info);

    argx_env(cli.arg, str("WHVN_API_KEY"), str("your API key"), &cli.api.key, 0, true);

    bool quit_early = false;
    TRYC(arg_parse(arg, argc, argv, &quit_early));

clean:
    whvn_collection_free(&cli.result.search);
    whvn_api_free(&cli.api);
    arg_free(&cli.arg);
    return err;
error:
    ERR_CLEAN;
}

