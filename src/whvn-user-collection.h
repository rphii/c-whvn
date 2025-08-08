#ifndef WHVN_USER_COLLECTION_H

#include <rlso.h>

typedef struct WhvnUserCollection {
    unsigned long id;
    So label;
    unsigned long views;
    bool is_public;
    unsigned long count;
} WhvnUserCollection, *WhvnUserCollections;

#define WHVN_USER_COLLECTION_H
#endif

