#include "json.h"

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
    if(json_parse_value(&p)) {
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

int main(void) {
    test_str(str("\"\""), true);
    test_str(str("\"asdf\""), true);
    test_str(str("\" asdf \""), true);
    test_str(str("\"\\n\""), true);
    test_str(str("\"\\uf37e\""), true);
    test_str(str("fail"), false);
    test_str(str(" fail"), false);
    test_str(str(""), false);
    test_bool(str(""), false);
    test_bool(str("true"), true);
    test_bool(str("false"), true);
    test_bool(str(" true"), false);
    test_bool(str("tru"), false);
    test_num(str("9"), true);
    test_num(str("123"), true);
    test_num(str("0.0"), true);
    test_num(str("-0.0"), true);
    test_num(str("-0.0e0"), true);
    test_num(str("-0.0e+0"), true);
    test_num(str("-0.0e-0"), true);
    test_num(str("-1023456789.01234e-0"), true);
    test_num(str("-912345670.919824e-0"), true);
    test_num(str("-0e-0"), true);
    test_num(str("1e123"), true);
    test_num(str("1e+123"), true);
    test_num(str("-0.e-0"), false);
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
}

