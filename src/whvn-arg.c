#include "whvn-arg.h"

void whvn_arg_init(struct Arg **arg_p) {
    *arg_p = arg_new();
    struct Arg *arg = *arg_p;
    arg_init(arg, str("c-whvn"), str("wallhaven api"), str("https://github.com/rphii/c-whvn"));

    struct ArgX *x = 0;
    struct ArgXGroup *g = 0;
    x=argx_init(arg_opt(arg), 'h', str("help"), str("display this help"));
      argx_help(x, arg);

    //x=argx_init(arg_opt(arg), '

}

ErrDecl whvn_arg_parse(struct Arg *arg, const int argc, const char **argv, bool *quit_early) {
    return arg_parse(arg, argc, argv, quit_early);
}

void whvn_arg_free(struct Arg **arg_p) {
    arg_free(arg_p);
}

