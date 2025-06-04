#ifndef WHVN_USER_COLLECTION_H

#include <rphii/str.h>

typedef struct WhvnUserCollection {
    Str id;
    Str label;
    unsigned long views;
    bool is_public;
    unsigned long count;
} WhvnUserCollection, *WhvnUserCollections;

#define WHVN_USER_COLLECTION_H
#endif

