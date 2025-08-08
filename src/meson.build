
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
  'whvn-ratio.h',
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
  ]

whvn_lib_src = [
  'whvn-api-search.c',
  'whvn-api.c',
  'whvn-category.c',
  'whvn-collection.c',
  'whvn-json-parse.c',
  'whvn-purity.c',
  'whvn-ratio.c',
  'whvn-resolution.c',
  'whvn-response.c',
  'whvn-tag.c',
  'whvn-toplist-range.c',
  'whvn-user-settings.c',
  'whvn-wallpaper-info.c',
  ]

whvn_cli_src = [
  'whvn-cli.c',
  ]

cc = meson.get_compiler('c')

rljson_dep = dependency('rljson', fallback : ['rljson', 'rljson_dep'], default_options: ['default_library=static'])
rlarg_dep = dependency('rlarg', fallback : ['rlarg', 'rlarg_dep'], default_options: ['default_library=static'])
rlso_dep = dependency('rlso', fallback : ['rlso', 'rlso_dep'], default_options: ['default_library=static'])
rlc_dep = dependency('rlc', fallback : ['rlc', 'rlc_dep'], default_options: ['default_library=static'])

curl_dep = dependency('libcurl', required: true)
m_dep = cc.find_library('m', required: false)

libwhvn = library('whvn',
  whvn_lib_src,
  dependencies: [rljson_dep, rlso_dep, rlc_dep, curl_dep],
  install: true,
)

whvn_dep = declare_dependency(
  link_with: libwhvn,
  include_directories: '.',
  )

install_headers(whvn_lib_headers, subdir: 'whvn')

whvn_cli = executable('whvn-cli',
  whvn_cli_src,
  dependencies: [rlc_dep, rlarg_dep, rlso_dep, curl_dep],
  link_with: libwhvn,
  install: true,
)

