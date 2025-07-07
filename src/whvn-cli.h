#ifndef WHVN_CLI_H

#include <rphii/arg.h>
#include "whvn-api.h"

typedef struct WhvnCli {
    struct Arg *arg;
    WhvnApi api;
    Str api_buf;
    VStr vbuf;
    WhvnApiSearch search;
    struct {
        Str wallpaper_info;
        Str tag_info;
        Str user_collection;
    } query;
    struct {
        bool print_pretty;
        bool open_browser;
        bool wait_user;
        bool download;
    } action;
    ssize_t max;
} WhvnCli;

#define WHVN_CLI_H
#endif

