#ifndef WHVN_RATIO_H

#include <rphii/str.h>

typedef enum {
    WHVN_RATIO,
    WHVN_RATIO_LANDSCAPE,
    WHVN_RATIO_PORTRAIT,
} WhvnRatioList;

#define WhvnRatioLandscape()    (WhvnRatio){ .type = WHVN_RATIO_LANDSCAPE }
#define WhvnRatioPortrait()     (WhvnRatio){ .type = WHVN_RATIO_PORTRAIT }

typedef struct WhvnRatio {
    unsigned int w;
    unsigned int h;
    WhvnRatioList type;
} WhvnRatio, *WhvnRatios;

void whvn_ratio_fmt(Str *out, WhvnRatio ratio);
void whvn_ratios_fmt(Str *out, WhvnRatios ratio);

#define WHVN_RATIO_H
#endif

