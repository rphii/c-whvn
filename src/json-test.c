#include "json.h"
#include <rphii/file.h>
#include "whvn-response.h"

bool json_parse_bool(JsonParse *p, bool *val);
bool json_parse_number(JsonParse *p, Str *val);

void test_str(Str s, bool expect_success) {
    printf("[%.*s] ", STR_F(s));
    JsonParse p = { .head = s };
    Str val = {0};
    if(json_parse_string(&p, &val)) {
        if(expect_success) {
            printf(F("[%.*s] parse ok\n", FG_GN_B), STR_F(val));
        } else {
            printf(F("[%.*s] expected fail\n", FG_YL), STR_F(val));
        }
    } else {
        if(!expect_success) {
            printf(F("failed as expected\n", FG_GN_B));
        } else {
            printf(F("expected success\n", FG_RD));
        }
    }
}

void test_bool(Str s, bool expect_success) {
    printf("[%.*s] ", STR_F(s));
    JsonParse p = { .head = s };
    bool val = false;
    if(json_parse_bool(&p, &val)) {
        if(expect_success) {
            printf(F("[%s] parse ok\n", FG_GN_B), val ? "true" : "false");
        } else {
            printf(F("[%s] expected fail\n", FG_YL), val ? "true" : "false");
        }
    } else {
        if(!expect_success) {
            printf(F("failed as expected\n", FG_GN_B));
        } else {
            printf(F("expected success\n", FG_RD));
        }
    }
}

void test_num(Str s, bool expect_success) {
    printf("[%.*s] ", STR_F(s));
    JsonParse p = { .head = s };
    Str val = {0};
    if(json_parse_number(&p, &val)) {
        if(expect_success) {
            printf(F("[%.*s] parse ok\n", FG_GN_B), STR_F(val));
        } else {
            printf(F("[%.*s] expected fail\n", FG_YL), STR_F(val));
        }
    } else {
        if(!expect_success) {
            printf(F("failed as expected\n", FG_GN_B));
        } else {
            printf(F("expected success\n", FG_RD));
        }
    }
}

void test_obj(Str s, bool expect_success) {
    printf("[%.*s] ", STR_F(s));
    JsonParse p = { .head = s };
    Str val = {0};
    if(json_parse_object(&p)) {
        if(expect_success) {
            printf(F("[%.*s] parse ok\n", FG_GN_B), STR_F(val));
        } else {
            printf(F("[%.*s] expected fail\n", FG_YL), STR_F(val));
        }
    } else {
        if(!expect_success) {
            printf(F("failed as expected\n", FG_GN_B));
        } else {
            printf(F("expected success\n", FG_RD));
        }
    }
}

void test_val(Str s, bool expect_success) {
    printf("[%.*s] ", STR_F(s));
    JsonParse p = { .head = s };
    Str val = {0};
    JsonParseValue v;
    if(json_parse_value(&p, &v)) {
        if(expect_success) {
            printf(F("[%.*s] parse ok\n", FG_GN_B), STR_F(val));
        } else {
            printf(F("[%.*s] expected fail\n", FG_YL), STR_F(val));
        }
    } else {
        if(!expect_success) {
            printf(F("failed as expected\n", FG_GN_B));
        } else {
            printf(F("expected success\n", FG_RD));
        }
    }
}


#include "whvn-wallpaper-info.h"

void *whvn_jparse_thumbs(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    // printf("   %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    return 0;
}

void *whvn_jparse_colors(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    // printf("   %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    return 0;
}

void *whvn_jparse_wallpaper_info(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    // printf("  %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("thumbs"))) return whvn_jparse_thumbs;
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("colors"))) return whvn_jparse_colors;
    if(val) {
    }
    return 0;
}

void *whvn_jparse_data(void **user, JsonParseValue key, JsonParseValue *val) {
    printf(" %s - '%.*s'\n", __func__, STR_F(json_parse_value_str(key)));
    WhvnWallpaperInfo **data = (WhvnWallpaperInfo **)user;
    _array_resize(data, __FILE__, __LINE__, __func__, sizeof(*data), array_len(*data) + 1);
    printf("resize %zu %p\n", array_len(*data), *data);
    return whvn_jparse_wallpaper_info;
}

void *whvn_jparse_meta(void **user, JsonParseValue key, JsonParseValue *val) {
    JsonParseValue v = val ? *val : (JsonParseValue){0};
    printf(" %s - '%.*s' : '%.*s'\n", __func__, STR_F(json_parse_value_str(key)), STR_F(json_parse_value_str(v)));
    if(val && user) {
        WhvnMeta *meta = (*(WhvnMeta **)user);
        if(!meta) assert(0);
        size_t z = 0;
        if(val->id == JSON_NUMBER) str_as_size(val->s, &z, 10);
        if(!str_cmp(key.s, str("current_page"))) meta->current_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("last_page"))) meta->last_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("per_page"))) meta->per_page = (unsigned long)z;
        else if(!str_cmp(key.s, str("total"))) meta->total = (unsigned long)z;
        else if(!str_cmp(key.s, str("query"))) meta->query = val->s;
        else if(!str_cmp(key.s, str("seed"))) {}
    }
    return 0;
}

void *whvn_jparse_response(void **user, JsonParseValue key, JsonParseValue *val) {
    // printf("%s - '%.*s'\n", __func__, STR_F(json_parse_value_str(key)));
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("data"))) {
        *user = (WhvnWallpaperInfo *)&(*(WhvnResponse **)user)->data;
        return whvn_jparse_data;
    }
    if(key.id == JSON_OBJECT && !str_cmp(key.s, str("meta"))) {
        *user = (WhvnMeta *)&(*(WhvnResponse **)user)->meta;
        return whvn_jparse_meta;
    }
    return 0;
}

int main(void) {

#if 0
    test_val(str("\"\""), true);
    test_val(str("\"asdf\""), true);
    test_val(str("\" asdf \""), true);
    test_val(str("\"\\n\""), true);
    test_val(str("\"\\uf37e\""), true);
    test_val(str("fail"), false);
    test_val(str(" fail"), false);
    test_val(str(""), false);
    test_val(str(""), false);
    test_val(str("true"), true);
    test_val(str("false"), true);
    test_val(str(" true"), false);
    test_val(str("tru"), false);
    test_val(str("9"), true);
    test_val(str("123"), true);
    test_val(str("0.0"), true);
    test_val(str("-0.0"), true);
    test_val(str("-0.0e0"), true);
    test_val(str("-0.0e+0"), true);
    test_val(str("-0.0e-0"), true);
    test_val(str("-1023456789.01234e-0"), true);
    test_val(str("-912345670.919824e-0"), true);
    test_val(str("-0e-0"), true);
    test_val(str("1e123"), true);
    test_val(str("1e+123"), true);
    test_val(str("-0.e-0"), false);
    test_obj(str("{}"), true);
    test_obj(str("{{}}}"), false);
    test_obj(str("{\"a\":\"b\"}}"), true);
    test_obj(str("{\"a\":{}}}"), true);
    test_obj(str("{\"a\":[]}}"), true);
    test_obj(str("{\"\":[{}]}}"), true);
    test_obj(str("{\"\":[[[[{}]]]]}}"), true);
    test_obj(str("{\"\":[[[[{\"x\"}]]]]}}"), false);
    test_val(str("[null]"), true);
    test_val(str("[null,0]"), true);
    test_val(str("[null,]"), false);
    test_val(str("[[] ]"), true);
    test_val(str("[\"\"]"), true);
    test_val(str("{ \"min\": -1.0e+28, \"max\": 1.0e+28 }"), true);
    test_val(str("{ \"min\": \"y\", \"max\": \"x\" }"), true);
    test_val(str_ll("[\0]", 3), false);
#endif

#if 1
    Str content = {0};
    WhvnResponse response = {0};

    file_str_read(str("test2.json"), &content);
    json_parse(content, whvn_jparse_response, &response);
    printf("meta.current_page %lu\n", response.meta.current_page);
    printf("meta.last_page %lu\n", response.meta.last_page);
    printf("meta.per_page %lu\n", response.meta.per_page);
    printf("meta.total %lu\n", response.meta.total);
    printf("meta.query %.*s\n", STR_F(response.meta.query));
    printf("meta.seed %.*s\n", STR_F(response.meta.seed ? *response.meta.seed : str("null")));
    printf("data len %zu\n", array_len(response.data));
    str_free(&content);
#endif

}

