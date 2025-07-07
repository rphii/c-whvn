
whvn_lib_headers = [
  'whvn-api-search.h',
  'whvn-api.h',
  'whvn-avatar.h',
  'whvn-category.h',
  'whvn-cli.h',
  'whvn-collection.h',
  'whvn-json-parse.h',
  'whvn-meta.h',
  'whvn-order.h',
  'whvn-purity.h',
  'whvn-resolution.h',
  'whvn-response.h',
  'whvn-sorting.h',
  'whvn-tag.h',
  'whvn-thumbs.h',
  'whvn-toplist-range.h',
  'whvn-uploader.h',
  'whvn-user-collection.h',
  'whvn-user-settings.h',
  'whvn-wallpaper-info.h',
  'whvn-ratio.h',
  ]

whvn_lib_src = [
  'whvn-api-search.c',
  'whvn-api.c',
  'whvn-category.c',
  'whvn-collection.c',
  'whvn-json-parse.c',
  'whvn-purity.c',
  'whvn-response.c',
  'whvn-tag.c',
  'whvn-toplist-range.c',
  'whvn-user-settings.c',
  'whvn-wallpaper-info.c',
  'whvn-ratio.c',
  ]

whvn_cli_src = [
  'whvn-cli.c',
  ]

cc = meson.get_compiler('c')
dep_rphiic = dependency('rphiic', required: true)
dep_curl = dependency('libcurl', required: true)
dep_m = cc.find_library('m', required: false)

whvn_lib = library('whvn',
  whvn_lib_src,
  dependencies: [dep_rphiic, dep_curl],
  install: true,
)

install_headers(whvn_lib_headers, subdir: 'whvn')

whvn_cli = executable('whvn-cli',
  whvn_cli_src,
  dependencies: [dep_rphiic, dep_curl],
  link_with: whvn_lib,
  install: true,
)
