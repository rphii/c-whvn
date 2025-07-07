#ifndef WHVN_RESOLUTION_H

#include <rphii/str.h>

typedef struct WhvnResolution {
    unsigned int w;
    unsigned int h;
} WhvnResolution, *WhvnResolutions;

bool whvn_resolution_is_valid(WhvnResolution res);
void whvn_resolution_fmt(Str *out, WhvnResolution res);
void whvn_resolutions_fmt(Str *out, WhvnResolutions res);
WhvnResolution whvn_resolution_parse(Str str);

#define WHVN_RESOLUTION_H
#endif

