#include "json.h"
#include <rphii/file.h>
#include "whvn-category.h"
#include "whvn-response.h"
#include "whvn-json-parse.h"

#if 0
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
#endif


#include "whvn-wallpaper-info.h"

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
    test_num(str("0.6"), true);
#endif

#if 1
    Str content = {0};
    file_str_read(str("/home/rphii/dev/nioh2-mods/mods/randomizer/randomizer-0.1.0/default-2x.json"), &content);
    json_parse(content, 0, 0);
#else
#if 1
    Str content = {0};
    WhvnResponse response = {0};
#if 0
    file_str_read(str("test.json"), &content);
    json_parse(content, whvn_jparse_data, &response.data);
    whvn_wallpaper_info_print(&response.data[0]);
    whvn_wallpaper_info_free(&response.data[0]);
    array_free(response.data);
#else 
    file_str_read(str("test3.json"), &content);
    json_parse(content, whvn_json_parse_response, &response);
    printf("data len %zu\n", array_len(response.data));
    for(size_t i = 0; i < array_len(response.data); ++i) {
        WhvnWallpaperInfo info = array_at(response.data, i);
        printf(F("### %zu : %.*s\n", BOLD UL), i, STR_F(info.id));
        whvn_wallpaper_info_print(&info);
        whvn_wallpaper_info_free(&info);
    }
    array_free(response.data);
    printf(F("### metadata\n", BOLD UL));
    printf("meta.current_page %lu\n", response.meta.current_page);
    printf("meta.last_page %lu\n", response.meta.last_page);
    printf("meta.per_page %lu\n", response.meta.per_page);
    printf("meta.total %lu\n", response.meta.total);
    printf("meta.query %.*s\n", STR_F(response.meta.query));
    printf("meta.seed %.*s\n", STR_F(response.meta.seed ? *response.meta.seed : str("null")));
    str_free(&response.meta.query);
    str_free(response.meta.seed);
#endif
    str_free(&content);
#endif
#endif

}

