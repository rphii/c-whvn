#include "whvn-json-parse.h"
#include "whvn.h"

void *whvn_json_parse_thumbs(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    WhvnThumbs *thumbs = *(WhvnThumbs **)user;
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    if(!str_cmp(key.s, str("small"))) json_fmt_str(&thumbs->small, v.s);
    if(!str_cmp(key.s, str("large"))) json_fmt_str(&thumbs->large, v.s);
    if(!str_cmp(key.s, str("original"))) json_fmt_str(&thumbs->original, v.s);
    return 0;
}

void *whvn_json_parse_colors(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    Color ***info = (Color ***)user;
    Color color = {0};
    str_as_color(v.s, &color);
    array_push(**info, color);
    return 0;
}

void *whvn_json_parse_tag(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    WhvnTag *tag = *(WhvnTag **)user;
    //return 0;
    size_t z = 0;
    if(v.id == JSON_NUMBER) str_as_size(v.s, &z, 10);
    if(!str_cmp(key.s, str("id"))) tag->id = z;
    if(!str_cmp(key.s, str("name"))) json_fmt_str(&tag->name, v.s);
    if(!str_cmp(key.s, str("alias"))) json_fmt_str(&tag->alias, v.s);
    if(!str_cmp(key.s, str("category_id"))) tag->category_id = z;
    if(!str_cmp(key.s, str("category"))) json_fmt_str(&tag->category, v.s);
    if(!str_cmp(key.s, str("purity"))) {
        WhvnPurityFlag purity = 0;
        if(!str_cmp(v.s, str("sfw"))) purity |= WHVN_PURITY_SFW;
        if(!str_cmp(v.s, str("nsfw"))) purity |= WHVN_PURITY_NSFW;
        if(!str_cmp(v.s, str("sketchy"))) purity |= WHVN_PURITY_SKETCHY;
        tag->purity = purity;
    }
    if(!str_cmp(key.s, str("created_at"))) json_fmt_str(&tag->created_at, v.s);
    return 0;
}

void *whvn_json_parse_tags(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    WhvnTags *tags = *(WhvnTags **)user;
    size_t len = array_len(*tags);
    array_resize(*tags, len + 1);
    *user = array_it(*tags, len);
    return whvn_json_parse_tag;
}

void *whvn_json_parse_avatar(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    WhvnAvatar *avatar = *(WhvnAvatar **)user;
    if(!str_cmp(key.s, str("200px"))) json_fmt_str(&avatar->px200, v.s);
    if(!str_cmp(key.s, str("128px"))) json_fmt_str(&avatar->px128, v.s);
    if(!str_cmp(key.s, str("32px"))) json_fmt_str(&avatar->px32, v.s);
    if(!str_cmp(key.s, str("20px"))) json_fmt_str(&avatar->px20, v.s);
    return 0;
} void *whvn_json_parse_uploader(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff("   %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    WhvnUploader *uploader = *(WhvnUploader **)user;
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("avatar"))) {
        *user = &uploader->avatar;
        return whvn_json_parse_avatar;
    }
    if(!str_cmp(key.s, str("username"))) json_fmt_str(&uploader->username, v.s);
    if(!str_cmp(key.s, str("group"))) json_fmt_str(&uploader->group, v.s);
    return 0;
}

void *whvn_json_parse_wallpaper_info(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    WhvnWallpaperInfo *info = (*(WhvnWallpaperInfo **)user);
    //printff("  %s - '%.*s' : '%.*s' %u", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)), v.id);
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("thumbs"))) {
        *user = &info->thumbs;
        return whvn_json_parse_thumbs;
    }
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("colors"))) {
        *user = &info->colors;
        return whvn_json_parse_colors;
    }
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("uploader"))) {
        *user = &info->uploader;
        return whvn_json_parse_uploader;
    }
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("tags"))) {
        *user = &info->tags;
        return whvn_json_parse_tags;
    }
    //return 0;
    if(val && user) {
        size_t z = 0;
        double d = 0;
        if(v.id == JSON_NUMBER) str_as_size(v.s, &z, 10);
        if(v.id == JSON_NUMBER) str_as_double(v.s, &d);
        //printff("val.id %u, key.id %u",v.id, key.id);
        if(!str_cmp(key.s, str("id"))) json_fmt_str(&info->id, v.s);
        else if(!str_cmp(key.s, str("url"))) json_fmt_str(&info->url, v.s);
        else if(!str_cmp(key.s, str("short_url"))) json_fmt_str(&info->short_url, v.s);
        else if(!str_cmp(key.s, str("views"))) info->views = (unsigned long)z;
        else if(!str_cmp(key.s, str("favorites"))) info->favorites = (unsigned long)z;
        else if(!str_cmp(key.s, str("source"))) json_fmt_str(&info->source, v.s);
        else if(!str_cmp(key.s, str("purity"))) {
            WhvnPurityFlag purity = 0;
            if(!str_cmp(v.s, str("sfw"))) purity |= WHVN_PURITY_SFW;
            if(!str_cmp(v.s, str("nsfw"))) purity |= WHVN_PURITY_NSFW;
            if(!str_cmp(v.s, str("sketchy"))) purity |= WHVN_PURITY_SKETCHY;
            info->purity = purity;
        }
        else if(!str_cmp(key.s, str("category"))) {
            WhvnCategoryFlag category = 0;
            if(!str_cmp(v.s, str("people"))) category |= WHVN_CATEGORY_PEOPLE;
            if(!str_cmp(v.s, str("anime"))) category |= WHVN_CATEGORY_ANIME;
            if(!str_cmp(v.s, str("general"))) category |= WHVN_CATEGORY_GENERAL;
            info->category = category;
        }
        else if(!str_cmp(key.s, str("dimension_x"))) info->dimension_x = (unsigned long)z;
        else if(!str_cmp(key.s, str("dimension_y"))) info->dimension_y = (unsigned long)z;
        else if(!str_cmp(key.s, str("resolution"))) json_fmt_str(&info->resolution, v.s);
        else if(!str_cmp(key.s, str("ratio"))) json_fmt_str(&info->ratio, v.s);
        else if(!str_cmp(key.s, str("file_size"))) info->file_size = (unsigned long)z;
        else if(!str_cmp(key.s, str("file_type"))) json_fmt_str(&info->file_type, v.s);
        else if(!str_cmp(key.s, str("created_at"))) json_fmt_str(&info->created_at, v.s);
        else if(!str_cmp(key.s, str("path"))) json_fmt_str(&info->path, v.s);
    }
    return 0;
}

void *whvn_json_parse_data(void **user, JsonParseValue key, JsonParseValue *val) {
    //printf(" %s - '%.*s'\n", __func__, STR_F(json_parse_value_str(key)));
    WhvnWallpaperInfo **data = *(WhvnWallpaperInfo ***)user;
    size_t len = array_len(*data);
    array_resize(*data, len + 1);
    *user = array_it(*data, len);
    return whvn_json_parse_wallpaper_info;
}

void *whvn_json_parse_meta(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    //printff(" %s - '%.*s' : '%.*s'", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    if(val && user) {
        WhvnMeta *meta = (*(WhvnMeta **)user);
        if(!meta) assert(0);
        size_t z = 0;
        if(val->id == JSON_NUMBER) str_as_size(val->s, &z, 10);
        if(!str_cmp(key.s, str("current_page"))) meta->current_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("last_page"))) meta->last_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("per_page"))) meta->per_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("total"))) meta->total = (unsigned long)z;
        else if(!str_cmp(key.s, str("query"))) json_fmt_str(&meta->query, val->s);
        else if(!str_cmp(key.s, str("seed"))) {}
    }
    return 0;
}

void *whvn_json_parse_response(void **user, JsonParseValue key, JsonParseValue *val) {
    //printff("%s - '%.*s'", __func__, STR_F(json_parse_value_str(key)));
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("data"))) {
        *user = (WhvnWallpaperInfo *)&(*(WhvnResponse **)user)->data;
        return whvn_json_parse_data;
    }
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("meta"))) {
        *user = (WhvnMeta *)&(*(WhvnResponse **)user)->meta;
        return whvn_json_parse_meta;
    }
    return 0;
}


