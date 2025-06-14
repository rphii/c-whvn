#ifndef WHVN_CLI_H

#include <rphii/arg.h>
#include "whvn-api.h"

typedef struct WhvnCli {
    struct Arg *arg;
    WhvnApi api;
    Str api_buf;
    WhvnApiSearch search;
    struct {
        Str wallpaper_info;
        Str tag_info;
        Str user_collection;
    } query;
    bool print_pretty;
    ssize_t max;
} WhvnCli;

#define WHVN_CLI_H
#endif

