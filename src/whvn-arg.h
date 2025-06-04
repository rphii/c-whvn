#ifndef WHVN_ARG_H

#include <rphii/err.h>
#include <rphii/arg.h>

typedef struct Whvn Whvn;

void whvn_arg_init(struct Arg **arg_p);

#define ERR_whvn_arg_parse(...) "argument parsing failed"
ErrDecl whvn_arg_parse(struct Arg *arg, const int argc, const char **argv, bool *quit_early);
void whvn_arg_free(struct Arg **arg_p);

#define WHVN_ARG_H
#endif

