#ifndef WHVN_UPLOADER_H

#include <rphii/str.h>
#include "whvn-avatar.h"

typedef struct WhvnUploader {
    Str username;
    Str group;
    WhvnAvatar avatar;
} WhvnUploader;

#define WHVN_UPLOADER_H
#endif

