#ifndef WHVN_JSON_PARSE_H

#include <rljson.h>
#include "whvn-api.h"

void *whvn_json_parse_thumbs(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_colors(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_tag(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_avatar(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_wallpaper_info(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_meta(void **user, Json_Parse_Value key, Json_Parse_Value *val);

void *whvn_json_parse_response(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_data_wallpaper_info(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_data_tag_info(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_data_user_settings(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_data_user_collections(void **user, Json_Parse_Value key, Json_Parse_Value *val);
void *whvn_json_parse_data_user_collection(void **user, Json_Parse_Value key, Json_Parse_Value *val);

#define WHVN_JSON_PARSE_H
#endif

