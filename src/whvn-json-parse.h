#ifndef WHVN_JSON_PARSE_H

#include <rphii/json.h>
#include "whvn-api.h"

void *whvn_json_parse_thumbs(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_colors(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_tag(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_avatar(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_wallpaper_info(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_meta(void **user, JsonParseValue key, JsonParseValue *val);

void *whvn_json_parse_response(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_data_wallpaper_info(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_data_tag_info(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_data_user_settings(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_data_user_collections(void **user, JsonParseValue key, JsonParseValue *val);
void *whvn_json_parse_data_user_collection(void **user, JsonParseValue key, JsonParseValue *val);

#define WHVN_JSON_PARSE_H
#endif

