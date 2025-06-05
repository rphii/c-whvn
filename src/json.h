#ifndef RPHIIC_JSON_H

#include <rphii/str.h>

typedef struct JsonParse JsonParse;

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
} JsonList;

typedef int (*JsonParseFunc)(JsonParse *p);
typedef int (*JsonParseCallbackKV)(void *user, Str key, Str val, JsonList id);
typedef int (*JsonParseCallbackObject)(void *user, Str key);
typedef int (*JsonParseCallbackArray)(void *user, Str key);

//typedef struct JsonParseFrame {
//    void *user;
//    JsonParseFunc *next;
//} JsonParseFrame;

typedef struct JsonParse {
    Str head;
    JsonParseFunc root;
} JsonParse;

typedef struct JsonParseValue {
    union {
        Str s;
        bool b;
    };
    JsonList id;
} JsonParseValue;

bool json_parse(JsonParse *p);
bool json_parse_value(JsonParse *p);
bool json_parse_object(JsonParse *p);
bool json_parse_array(JsonParse *p);
bool json_parse_null(JsonParse *p);
bool json_parse_bool(JsonParse *p, bool *val);
bool json_parse_string(JsonParse *p, Str *val);
bool json_parse_number(JsonParse *p, Str *val);

#define RPHIIC_JSON_H
#endif

