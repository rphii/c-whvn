#ifndef WHVN_RATIO_H

#include <rlso.h>

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

void whvn_ratio_fmt(So *out, WhvnRatio ratio);
void whvn_ratios_fmt(So *out, WhvnRatios ratio);
WhvnRatio whvn_ratio_parse(So str);

#define WHVN_RATIO_H
#endif

