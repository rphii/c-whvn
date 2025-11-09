#ifndef WHVN_CLI_H

#include <rlarg.h>
#include "whvn-api.h"

typedef struct WhvnCli {
    struct Arg *arg;
    WhvnApi api;
    So api_buf;
    So download_root;
    VSo vbuf_ratios;
    So buf_atleast;
    VSo vbuf_resolutions;
    WhvnApiSearch search;
    struct {
        So wallpaper_info;
        So tag_info;
        So user_collection;
    } query;
    struct {
        bool print_pretty;
        bool print_tags;
        bool open_browser;
        bool wait_user;
        bool download;
    } action;
    ssize_t max;
} WhvnCli;

#define WHVN_CLI_H
#endif

