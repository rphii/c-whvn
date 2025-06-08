#ifndef RPHIIC_JSON_H

#include <rphii/str.h>

#define JSON_DEPTH_MAX  4096

typedef struct JsonParse JsonParse;

typedef enum {
    //JSON_NONE,
    /* keep below */
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
} JsonList;

//typedef struct JsonParseFrame {
//    void *user;
//    JsonParseFunc *next;
//} JsonParseFrame;

typedef struct JsonParseValue {
    union {
        Str s;
        bool b;
    };
    JsonList id;
} JsonParseValue;

typedef struct JsonParseSettings {
    bool verbose;
} JsonParseSettings;

typedef void *(*JsonParseCallback)(void **user, JsonParseValue key, JsonParseValue *val);

typedef struct JsonParse {
    Str head;
    JsonParseValue key;
    size_t depth;
    JsonParseCallback callback;
    JsonParseSettings settings;
    void *user;
} JsonParse;

Str json_parse_value_str(JsonParseValue v);

ErrDecl json_parse_valid(Str input);

#define ERR_json_parse(...) "failed parsing json (invalid input)"
ErrDecl json_parse(Str input, JsonParseCallback callback, void *user);

#if 0
bool json_parse_object(JsonParse *p);
bool json_parse_array(JsonParse *p);
bool json_parse_value(JsonParse *p, JsonParseValue *v);
/* atomic */
bool json_parse_null(JsonParse *p);
bool json_parse_bool(JsonParse *p, bool *val);
bool json_parse_string(JsonParse *p, Str *val);
bool json_parse_number(JsonParse *p, Str *val);
#endif

void json_fmt_str(Str *out, Str json_str); /* creates a new string; additional memory allocation */
void json_fix_str(Str *out, Str json_str); /* modifies the existing string; no additional memory allocation */

#define RPHIIC_JSON_H
#endif

