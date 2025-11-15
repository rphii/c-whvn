#include "whvn-json-parse.h"

void whvn_json_value_parse_purity(Json_Parse_Value v, WhvnPurity *purity) {
    if(!so_cmp(v.s, so("sfw"))) purity->sfw = true;
    if(!so_cmp(v.s, so("nsfw"))) purity->nsfw = true;
    if(!so_cmp(v.s, so("sketchy"))) purity->sketchy = true;
}

void whvn_json_value_parse_category(Json_Parse_Value v, WhvnCategory *category) {
    if(!so_cmp(v.s, so("people"))) category->people = true;
    if(!so_cmp(v.s, so("anime"))) category->anime = true;
    if(!so_cmp(v.s, so("general"))) category->general = true;
}

WhvnToplistRangeList whvn_json_value_parse_toplist_range(Json_Parse_Value v) {
    WhvnToplistRangeList range = WHVN_TOPLIST_RANGE_NONE;
    if(!so_cmp(v.s, so("1d"))) range = WHVN_TOPLIST_RANGE_1D;
    if(!so_cmp(v.s, so("3d"))) range = WHVN_TOPLIST_RANGE_3D;
    if(!so_cmp(v.s, so("1w"))) range = WHVN_TOPLIST_RANGE_1W;
    if(!so_cmp(v.s, so("1M"))) range = WHVN_TOPLIST_RANGE_1M;
    if(!so_cmp(v.s, so("3M"))) range = WHVN_TOPLIST_RANGE_3M;
    if(!so_cmp(v.s, so("6M"))) range = WHVN_TOPLIST_RANGE_6M;
    if(!so_cmp(v.s, so("1Y"))) range = WHVN_TOPLIST_RANGE_1Y;
    return range;
}

void *whvn_json_parse_thumbs(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    WhvnThumbs *thumbs = *(WhvnThumbs **)user;
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    if(!so_cmp(key.s, so("small"))) json_fix_so(&thumbs->small, v.s);
    if(!so_cmp(key.s, so("large"))) json_fix_so(&thumbs->large, v.s);
    if(!so_cmp(key.s, so("original"))) json_fix_so(&thumbs->original, v.s);
    return 0;
}

void *whvn_json_parse_colors(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    Color ***info = (Color ***)user;
    Color color = {0};
    so_as_color(v.s, &color);
    array_push(**info, color);
    return 0;
}

void *whvn_json_parse_tag(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    WhvnTag *tag = *(WhvnTag **)user;
    //return 0;
    size_t z = 0;
    if(v.id == JSON_NUMBER) so_as_size(v.s, &z, 10);
    if(!so_cmp(key.s, so("id"))) tag->id = z;
    if(!so_cmp(key.s, so("name"))) json_fix_so(&tag->name, v.s);
    if(!so_cmp(key.s, so("alias"))) json_fix_so(&tag->alias, v.s);
    if(!so_cmp(key.s, so("category_id"))) tag->category_id = z;
    if(!so_cmp(key.s, so("category"))) json_fix_so(&tag->category, v.s);
    if(!so_cmp(key.s, so("purity"))) whvn_json_value_parse_purity(v, &tag->purity);
    if(!so_cmp(key.s, so("created_at"))) json_fix_so(&tag->created_at, v.s);
    return 0;
}

void *whvn_json_parse_tags(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    WhvnTags *tags = *(WhvnTags **)user;
    size_t len = array_len(*tags);
    array_resize(*tags, len + 1);
    *user = array_it(*tags, len);
    return whvn_json_parse_tag;
}

void *whvn_json_parse_avatar(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    WhvnAvatar *avatar = *(WhvnAvatar **)user;
    if(!so_cmp(key.s, so("200px"))) json_fix_so(&avatar->px200, v.s);
    if(!so_cmp(key.s, so("128px"))) json_fix_so(&avatar->px128, v.s);
    if(!so_cmp(key.s, so("32px"))) json_fix_so(&avatar->px32, v.s);
    if(!so_cmp(key.s, so("20px"))) json_fix_so(&avatar->px20, v.s);
    return 0;
}

void *whvn_json_parse_uploader(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)));
    WhvnUploader *uploader = *(WhvnUploader **)user;
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("avatar"))) {
        *user = &uploader->avatar;
        return whvn_json_parse_avatar;
    }
    if(!so_cmp(key.s, so("username"))) json_fix_so(&uploader->username, v.s);
    if(!so_cmp(key.s, so("group"))) json_fix_so(&uploader->group, v.s);
    return 0;
}

void *whvn_json_parse_wallpaper_info(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    WhvnWallpaperInfo *info = (*(WhvnWallpaperInfo **)user);
    //printff("  %s - '%.*s' : '%.*s' %u", __func__, STR_F(json_parse_value_so(key)), STR_F(json_parse_value_so(v)), v.id);
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("thumbs"))) {
        *user = &info->thumbs;
        return whvn_json_parse_thumbs;
    }
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("colors"))) {
        *user = &info->colors;
        return whvn_json_parse_colors;
    }
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("uploader"))) {
        *user = &info->uploader;
        return whvn_json_parse_uploader;
    }
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("tags"))) {
        *user = &info->tags;
        return whvn_json_parse_tags;
    }
    //return 0;
    if(val && user) {
        size_t z = 0;
        if(v.id == JSON_NUMBER) so_as_size(v.s, &z, 10);
        double d = 0;
        if(v.id == JSON_NUMBER) so_as_double(v.s, &d);
        //printff("val.id %u, key.id %u",v.id, key.id);
        if(!so_cmp(key.s, so("id"))) json_fix_so(&info->id, v.s);
        else if(!so_cmp(key.s, so("url"))) json_fix_so(&info->url, v.s);
        else if(!so_cmp(key.s, so("short_url"))) json_fix_so(&info->short_url, v.s);
        else if(!so_cmp(key.s, so("views"))) info->views = (unsigned long)z;
        else if(!so_cmp(key.s, so("favorites"))) info->favorites = (unsigned long)z;
        else if(!so_cmp(key.s, so("source"))) json_fix_so(&info->source, v.s);
        else if(!so_cmp(key.s, so("purity"))) whvn_json_value_parse_purity(v, &info->purity);
        else if(!so_cmp(key.s, so("category"))) whvn_json_value_parse_category(v, &info->category);
        else if(!so_cmp(key.s, so("dimension_x"))) info->dimension_x = (unsigned long)z;
        else if(!so_cmp(key.s, so("dimension_y"))) info->dimension_y = (unsigned long)z;
        else if(!so_cmp(key.s, so("resolution"))) json_fix_so(&info->resolution, v.s);
        else if(!so_cmp(key.s, so("ratio"))) json_fix_so(&info->ratio, v.s);
        else if(!so_cmp(key.s, so("file_size"))) info->file_size = (unsigned long)z;
        else if(!so_cmp(key.s, so("file_type"))) json_fix_so(&info->file_type, v.s);
        else if(!so_cmp(key.s, so("created_at"))) json_fix_so(&info->created_at, v.s);
        else if(!so_cmp(key.s, so("path"))) json_fix_so(&info->path, v.s);
    }
    return 0;
}

void *whvn_json_parse_data_wallpaper_info(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(key.id == JSON_OBJECT) {
        return whvn_json_parse_wallpaper_info;
    }
    if(key.id == JSON_ARRAY) {
        WhvnWallpaperInfos *data = *(WhvnWallpaperInfos **)user;
        size_t len = array_len(*data);
        array_resize(*data, len + 1);
        *user = array_it(*data, len);
        return whvn_json_parse_wallpaper_info;
    }
    return 0;
}

//printf(" %s - '%.*s'\n", __func__, STR_F(json_parse_value_so(key)));
    
void *whvn_json_parse_tag_info(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(key.id != JSON_OBJECT) return 0;
    if(!val) return 0;
    Json_Parse_Value v = *val;
    WhvnTag *tag = *(WhvnTag **)user;
    size_t z = 0;
    if(v.id == JSON_NUMBER) so_as_size(v.s, &z, 10);
    if(!so_cmp(key.s, so("id"))) tag->id = z;
    else if(!so_cmp(key.s, so("name"))) json_fix_so(&tag->name, v.s);
    else if(!so_cmp(key.s, so("alias"))) json_fix_so(&tag->alias, v.s);
    else if(!so_cmp(key.s, so("category_id"))) tag->category_id = z;
    else if(!so_cmp(key.s, so("category"))) json_fix_so(&tag->category, v.s);
    else if(!so_cmp(key.s, so("purity"))) whvn_json_value_parse_purity(v, &tag->purity);
    else if(!so_cmp(key.s, so("created_at"))) json_fix_so(&tag->created_at, v.s);
    return 0;
}

void *whvn_json_parse_data_tag_info(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(!so_cmp(key.s, so("data"))) {
        return whvn_json_parse_tag_info;
    }
    return 0;
}

void *whvn_json_parse_user_settings_purity(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    //Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printf(" %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_so(key)), STR_F(v.s));
    if(!val) return 0;
    WhvnUserSettings *settings = *(WhvnUserSettings **)user;
    whvn_json_value_parse_purity(*val, &settings->purity);
    return 0;
}

void *whvn_json_parse_user_settings_categories(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    //Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printf(" %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_so(key)), STR_F(v.s));
    if(!val) return 0;
    WhvnUserSettings *settings = *(WhvnUserSettings **)user;
    whvn_json_value_parse_category(*val, &settings->categories);
    return 0;
}

void *whvn_json_parse_user_settings_stringvec(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(!val) return 0;
    if(!so_len(val->s)) return 0;
    VSo *vec = *(VSo **)user;
    size_t len = array_len(*vec);
    array_resize(*vec, len + 1);
    So *s = array_it(*vec, len);
    json_fix_so(s, val->s);
    return 0;
}

void *whvn_json_parse_user_settings(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    Json_Parse_Value v = val ? *val : (Json_Parse_Value){0};
    //printf(" %s - '%.*s'\n", __func__, STR_F(json_parse_value_so(key)));
    WhvnUserSettings *settings = *(WhvnUserSettings **)user;
    size_t z = 0;
    if(v.id == JSON_NUMBER) so_as_size(v.s, &z, 10);
    if(!so_cmp(key.s, so("thumb_size"))) json_fix_so(&settings->thumb_size, v.s);
    if(!so_cmp(key.s, so("per_page"))) {
        so_as_size(v.s, &z, 10);
        settings->per_page = z;
    }
    if(!so_cmp(key.s, so("purity"))) return whvn_json_parse_user_settings_purity;
    if(!so_cmp(key.s, so("categories"))) return whvn_json_parse_user_settings_categories;
    if(!so_cmp(key.s, so("resolutions"))) {
        *user = &settings->resolutions;
        return whvn_json_parse_user_settings_stringvec;
    }
    if(!so_cmp(key.s, so("aspect_ratios"))) {
        *user = &settings->aspect_ratios;
        return whvn_json_parse_user_settings_stringvec;
    }
    if(!so_cmp(key.s, so("toplist_range"))) settings->toplist_range = whvn_json_value_parse_toplist_range(v);
    if(!so_cmp(key.s, so("tag_blacklist"))) {
        *user = &settings->tag_blacklist;
        return whvn_json_parse_user_settings_stringvec;
    }
    if(!so_cmp(key.s, so("user_blacklist"))) {
        *user = &settings->user_blacklist;
        return whvn_json_parse_user_settings_stringvec;
    }
    if(!so_cmp(key.s, so("ai_art_filter"))) settings->ai_art_filter = (bool)z;
    return 0;
}

void *whvn_json_parse_data_user_settings(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(!so_cmp(key.s, so("data"))) {
        return whvn_json_parse_user_settings;
    }
    return 0;
}

void *whvn_json_parse_user_collection(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(!val) return 0;
    WhvnUserCollection *collection = *(WhvnUserCollection **)user;
    size_t z = 0;
    if(val->id == JSON_NUMBER) so_as_size(val->s, &z, 10);
    if(!so_cmp(key.s, so("id"))) collection->id = z;
    else if(!so_cmp(key.s, so("label"))) json_fix_so(&collection->label, val->s);
    else if(!so_cmp(key.s, so("views"))) collection->views = z;
    else if(!so_cmp(key.s, so("public"))) collection->is_public = z;
    else if(!so_cmp(key.s, so("count"))) collection->count = z;
    return 0;
}

void *whvn_json_parse_user_collections(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    WhvnUserCollections *collections = *(WhvnUserCollections **)user;
    size_t len = array_len(*collections);
    array_resize(*collections, len + 1);
    *user = (WhvnUserCollection *)array_it(*collections, len);
    return whvn_json_parse_user_collection;
}

void *whvn_json_parse_data_user_collections(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(!so_cmp(key.s, so("data"))) {
        return whvn_json_parse_user_collections;
    }
    return 0;
}


void *whvn_json_parse_meta(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(key.id != JSON_OBJECT) return 0;
    if(!val) return 0;
    WhvnMeta *meta = (*(WhvnMeta **)user);
    if(!meta) assert(0);
    size_t z = 0;
    if(val->id == JSON_NUMBER) so_as_size(val->s, &z, 10);
    if(!so_cmp(key.s, so("current_page"))) meta->current_page = (unsigned long)z;
    else if(!so_cmp(key.s, so("last_page"))) meta->last_page = (unsigned long)z;
    else if(!so_cmp(key.s, so("per_page"))) meta->per_page = (unsigned long)z;
    else if(!so_cmp(key.s, so("total"))) meta->total = (unsigned long)z;
    else if(!so_cmp(key.s, so("query"))) json_fix_so(&meta->query, val->s);
    else if(!so_cmp(key.s, so("seed"))) {}
    return 0;
}

    //printff("%s - '%.*s'", __func__, STR_F(json_parse_value_so(key)));
void *whvn_json_parse_response(void **user, Json_Parse_Value key, Json_Parse_Value *val) {
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("data"))) {
        *user = (WhvnWallpaperInfo *)&(*(WhvnResponse **)user)->data;
        return whvn_json_parse_data_wallpaper_info;
    }
    if(key.id == JSON_OBJECT && !so_cmp(key.s, so("meta"))) {
        *user = (WhvnMeta *)&(*(WhvnResponse **)user)->meta;
        return whvn_json_parse_meta;
    }
    return 0;
}


