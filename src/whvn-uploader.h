#ifndef WHVN_UPLOADER_H

#include <rlso.h>
#include "whvn-avatar.h"

typedef struct WhvnUploader {
    So username;
    So group;
    WhvnAvatar avatar;
} WhvnUploader;

#define WHVN_UPLOADER_H
#endif

