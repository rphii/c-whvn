
whvn_cli_src = [
  'json.c',
  'whvn-api.c',
  'whvn-category.c',
  'whvn-cli.c',
  'whvn-collection.c',
  'whvn-json-parse.c',
  'whvn-purity.c',
  'whvn-response.c',
  'whvn-tag.c',
  'whvn-toplist-range.c',
  'whvn-wallpaper-info.c',
  ]

cc = meson.get_compiler('c')
dep_rphiic = dependency('rphiic', required: true)
dep_curl = dependency('libcurl', required: true)
dep_m = cc.find_library('m', required: false)

whvn_cli = executable('whvn-cli',
  whvn_cli_src,
  dependencies: [dep_rphiic, dep_curl],
  install: true,
  )

