# whvn

Wallhaven API library and cli

## dependencies

- [libcurl](https://curl.haxx.se/)
- [rphiic](https://github.com/rphii/rphiic)

## install

```sh
https://github.com/rphii/c-whvn && cd c-whvn
meson setup build
meson install -C build
```

installation comes with:
- whvn-cli : executable
- libwhvn.\{so,dll,etc\} : library

## whvn-cli

```c
whvn-cli: wallhaven API cli
Usage:
  build/src/whvn-cli api-call [options]
      api-call <wallpaper-info|search|tag-info|user-settings|user-collections|user-collection> 
          select api call
Options:
  -h  --help <arg>                            display this help
  -U  --url <string>                          api URL =https://wallhaven.cc/api/v1/
  -P  --print <url|response|pretty>           print the raw API response
  -c  --categories <general|anime|people>     search: categories
  -p  --purity <sfw|sketchy|nsfw>             search: purity
  -s  --sorting <date_added|relevance|random|views|favorites|toplist> 
          search: sorting
  -o  --order <asc|desc>                      search: order
  -t  --toplist-range <1d|3d|1w|1M|3M|6M|1y>  search: toplist range
  -i  --page <int>                            search: page =0
  -r  --seed <string>                         search: seed 
Environment Variables:
  WHVN_API_KEY                                your API key 
https://github.com/rphii/c-whvn

```

## usage as library

### example code (cli tool)

See example code: [whvn-cli.c](src/whvn-cli.c).

Gets installed, but you could also compile it as such: `gcc whvn-cli.c -lwhvn -lrphiic`

### api functions

This will probably be your main interface with the library.

```c
/* whvn-api.h */
ErrDecl whvn_api_wallpaper_info(WhvnApi *api, Str arg, Str *buf, WhvnWallpaperInfo *info);
ErrDecl whvn_api_search(WhvnApi *api, WhvnApiSearch *arg, Str *buf, WhvnResponse *response);
ErrDecl whvn_api_tag_info(WhvnApi *api, Str arg, Str *buf, WhvnTag *tag_info);
ErrDecl whvn_api_user_settings(WhvnApi *api, Str *buf, WhvnUserSettings *settings);
ErrDecl whvn_api_user_collections(WhvnApi *api, size_t page, Str *buf, WhvnUserCollections *collections);
ErrDecl whvn_api_user_collection(WhvnApi *api, size_t page, Str user, unsigned long id, Str *buf, WhvnResponse *response);
```

### using an api function

Example doing a search:

```c
    Str buf = {0}; /* initialize to 0! - can be re-used for subsequent API function calls */
    WhvnResponse response = {0}; /* initialize to 0! */
    WhvnApi api = {
        /* ... configure ... */
        .url = str("https://wallhaven.cc/api/v1/"),
        .key = str("your-api-key"),
    }; 
    WhvnApiSearch search = {
        /* ... configure search ... */
    };
    int result = whvn_api_search(&api, &search, &buf, &response);
    if(!result) {
        /* ...
        do something with 'response'
        ... */
    }
    whvn_response_free(&response); /* see chapter: ### memory freeing */
    str_free(&buf);
    return result;
```

### memory freeing

From everything discussed so far, the following needs to be freed if you're done with everything:

```c
whvn_wallpaper_info_free(&info);
whvn_response_free(&response);
whvn_user_settings_free(&settings);
array_free(collections);

whvn_api_free(&api);
str_free(&buf);
```

**!!! IMPORTANT !!!** >>> do NOT free your `buf` until you're done with any of
the results mentioned in [api functions](#api-functions). The `buf` is the API
answer in plaintext JSON format (albeit unusable - it will be a garbage JSON
string, since it gets modified, to fix strings; e.g. escaping characters) - the
described api functions merely construct the URL and use libCURL to get and
convert said raw string into the appropriate structs. `whvn`'s aim is to handle
data as efficiently as possible, so we don't want to allocate dynamic memory
when we don't have to - and one such case is any `Str` within the resulting
structures, because they are all just - fixed (un-escpated) - references to the
response string.

