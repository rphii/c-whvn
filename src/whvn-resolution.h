#ifndef WHVN_RESOLUTION_H

#include <rlso.h>

typedef struct WhvnResolution {
    unsigned int w;
    unsigned int h;
} WhvnResolution, *WhvnResolutions;

bool whvn_resolution_is_valid(WhvnResolution res);
void whvn_resolution_fmt(So *out, WhvnResolution res);
void whvn_resolutions_fmt(So *out, WhvnResolutions res);
WhvnResolution whvn_resolution_parse(So str);

#define WHVN_RESOLUTION_H
#endif

