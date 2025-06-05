#include "json.h"

#define JSON_DIGITS  str("0123456789")
#define JSON_DIGIT1  str("123456789")

/* return true on match */
bool json_parse_ch(JsonParse *p, char c) {
    ASSERT_ARG(p);
    if(!str_len(p->head)) return false;
    bool result = (bool)(*p->head.str == c);
    if(result) {
        ++p->head.str;
        --p->head.len;
    }
    return result;
}

/* return true on match */
bool json_parse_any(JsonParse *p, StrC s) {
    ASSERT_ARG(p);
    if(!str_len(p->head)) return false;
    char *result = strchr(s.str, *p->head.str);
    if(result && *result) {
        ++p->head.str;
        --p->head.len;
        return true;
    }
    return false;
}

void json_parse_ws(JsonParse *p) {
    ASSERT_ARG(p);
    while(json_parse_any(p, str(" \t\v\n"))) {}
}

/* return true on successful parse */
bool json_parse_string(JsonParse *p, Str *val) {
    ASSERT_ARG(p);
    ASSERT_ARG(val);
    JsonParse q = *p;
    if(!json_parse_ch(&q, '"')) return false;
    int escape = 0;
    while(str_len(q.head)) {
        if(escape < 0) {
            escape = 0;
            switch(*q.head.str) {
                case '"' : break;
                case '\\': break;
                case '/' : break;
                case 'b' : break;
                case 'f' : break;
                case 'n' : break;
                case 'r' : break;
                case 't' : break;
                case 'u' : escape += 4; break;
                default  : return false;
            }
            ++q.head.str;
            --q.head.len;
        } else if(escape) {
            if(!json_parse_any(&q, str("0123456789abcdefABCDEF"))) return false;
            --escape;
        } else if(json_parse_ch(&q, '\\')) {
            escape = -1;
        } else if(json_parse_ch(&q, '"')) {
            *val = str_ll(p->head.str + 1, q.head.str - p->head.str - 2);
            *p = q;
            return true;
        } else if(*q.head.str >= ' ') {
            ++q.head.str;
            --q.head.len;
        } else {
            return false;
        }
    }
    return false;
}

bool json_parse_bool(JsonParse *p, bool *val) {
    ASSERT_ARG(p);
    ASSERT_ARG(val);
    JsonParse q = *p;
    if(json_parse_ch(&q, 't')) {
        if(!json_parse_ch(&q, 'r')) return false;
        if(!json_parse_ch(&q, 'u')) return false;
        if(!json_parse_ch(&q, 'e')) return false;
        *val = true;
        *p = q;
        return true;
    }
    if(json_parse_ch(&q, 'f')) {
        if(!json_parse_ch(&q, 'a')) return false;
        if(!json_parse_ch(&q, 'l')) return false;
        if(!json_parse_ch(&q, 's')) return false;
        if(!json_parse_ch(&q, 'e')) return false;
        *val = false;
        *p = q;
        return true;
    }
    return false;
}


bool json_parse_number(JsonParse *p, Str *val) {
    ASSERT_ARG(p);
    ASSERT_ARG(val);
    JsonParse q = *p;
    json_parse_ch(&q, '-');
    if(json_parse_ch(&q, '0')) {
    } else if(json_parse_any(&q, JSON_DIGIT1)) {
        while(json_parse_any(&q, JSON_DIGITS)) {}
    } else {
        return false;
    }
    if(json_parse_ch(&q, '.')) {
        if(!json_parse_any(&q, JSON_DIGITS)) return false;
        while(json_parse_any(&q, JSON_DIGITS)) {}
    }
    if(json_parse_any(&q, str("eE"))) {
        json_parse_any(&q, str("+-"));
        if(!json_parse_any(&q, JSON_DIGITS)) return false;
        while(json_parse_any(&q, JSON_DIGITS)) {}
    }
    Str result = str_ll(p->head.str, q.head.str - p->head.str);
    size_t len = str_len(result);
    if(len) {
        *val = result;
        *p = q;
    }
    return (bool)len;
}

bool json_parse_null(JsonParse *p) {
    ASSERT_ARG(p);
    JsonParse q = *p;
    if(json_parse_ch(&q, 'n')) {
        if(!json_parse_ch(&q, 'u')) return false;
        if(!json_parse_ch(&q, 'l')) return false;
        if(!json_parse_ch(&q, 'l')) return false;
        *p = q;
        return true;
    }
    return false;
}

bool json_parse_value(JsonParse *p) {
    JsonParseValue v = {0};
    JsonParse q = *p;
    json_parse_ws(&q);
    if(json_parse_string(&q, &v.s)) goto valid;
    if(json_parse_number(&q, &v.s)) goto valid;
    if(json_parse_object(&q)) goto valid;
    if(json_parse_array(&q)) goto valid;
    if(json_parse_bool(&q, &v.b)) goto valid;
    if(json_parse_null(&q)) goto valid;
    return false;
valid:
    json_parse_ws(&q);
    *p = q;
    return true;
}

bool json_parse_array(JsonParse *p) {
    ASSERT_ARG(p);
    JsonParse q = *p;
    if(!json_parse_ch(&q, '[')) return false;
    json_parse_ws(&q);
    bool first = true;
    do {
        if(!json_parse_value(&q)) {
            if(first) break;
            else return false;
        }
        if(!json_parse_ch(&q, ',')) break;
        first = false;
    } while(str_len(q.head));
    if(!json_parse_ch(&q, ']')) return false;
    *p = q;
    return true;
}

bool json_parse_object(JsonParse *p) {
    ASSERT_ARG(p);
    JsonParse q = *p;
    if(!json_parse_ch(&q, '{')) return false;
    json_parse_ws(&q);
    JsonParseValue k = {0};
    bool first = true;
    do {
        json_parse_ws(&q);
        if(!json_parse_string(&q, &k.s)) {
            if(first) break;
            else return false;
        }
        json_parse_ws(&q);
        if(!json_parse_ch(&q, ':')) return false;
        if(!json_parse_value(&q)) return false;
        if(!json_parse_ch(&q, ',')) break;
        printff("PARSE OBJECT AGAIN!");
        first = false;
    } while(str_len(q.head));
    if(!json_parse_ch(&q, '}')) return false;
    *p = q;
    return true;
}

bool json_parse(JsonParse *p) {
    ASSERT_ARG(p);
    JsonParse q = *p;
    json_parse_ws(&q);
    if(json_parse_value(&q)) goto valid;
    return false;
valid:
    json_parse_ws(&q);
    return !str_len(q.head);
}

