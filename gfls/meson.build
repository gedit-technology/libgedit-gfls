gfls_public_headers = [
  'gfls.h',
  'gfls-attribute-keys.h',
  'gfls-init.h',
  'gfls-input-stream.h',
  'gfls-loader-basic.h',
  'gfls-loader-config.h',
  'gfls-loader-config-simple.h',
  'gfls-unsaved-document-titles.h',
  'gfls-utf8.h',
]

gfls_public_c_files = [
  'gfls-attribute-keys.c',
  'gfls-init.c',
  'gfls-input-stream.c',
  'gfls-loader-basic.c',
  'gfls-loader-config.c',
  'gfls-loader-config-simple.c',
  'gfls-unsaved-document-titles.c',
  'gfls-utf8.c',
]

headers_install_dir = get_option('includedir') / '@0@-@1@/gfls/'.format(meson.project_name(), GFLS_API_VERSION)
install_headers(
  gfls_public_headers,
  install_dir: headers_install_dir
)

gfls_enum_types = GNOME.mkenums_simple(
  'gfls-enum-types',
  decorator: 'G_MODULE_EXPORT',
  header_prefix: '#include <gmodule.h>',
  sources: gfls_public_headers,
  install_header: true,
  install_dir: headers_install_dir
)

gfls_static_lib = static_library(
  'libgedit-gfls-static',
  [gfls_public_c_files,
   gfls_enum_types],
  pic: true, # gfls_static_lib is potentially linked in a shared library.
  include_directories: ROOT_INCLUDE_DIR,
  dependencies: GFLS_DEPS,
  c_args: '-DGFLS_COMPILATION'
)

# For unit tests, to be able to test private functions.
GFLS_STATIC_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: gfls_static_lib,
  sources: gfls_enum_types[1],
  dependencies: GFLS_DEPS
)

symbol_map = 'symbol.map'
gfls_lib_link_args = c_compiler.get_supported_link_arguments([
  '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), symbol_map),
])

gfls_lib = library(
  'gedit-gfls-@0@'.format(GFLS_API_VERSION),
  dependencies: GFLS_DEPS,
  link_args: gfls_lib_link_args,
  link_depends: symbol_map,
  # link_whole is not supported with MSVC, so we use extract_all_objects().
  objects: gfls_static_lib.extract_all_objects(recursive: false),
  soversion: 0,
  install: true
)

GFLS_LIB_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: gfls_lib,
  sources: gfls_enum_types[1],
  dependencies: GFLS_DEPS
)

PKG_CONFIG.generate(gfls_lib,
  filebase: '@0@-@1@'.format(meson.project_name(), GFLS_API_VERSION),
  name: meson.project_name(),
  description: 'File loading and saving',
  subdirs: '@0@-@1@'.format(meson.project_name(), GFLS_API_VERSION),
  libraries: GFLS_DEPS,
)

if get_option('gobject_introspection')
  GNOME.generate_gir(
    gfls_lib,
    export_packages: '@0@-@1@'.format(meson.project_name(), GFLS_API_VERSION),
    header: 'gfls/gfls.h',
    identifier_prefix: 'Gfls',
    include_directories: ROOT_INCLUDE_DIR,
    includes: ['Gtk-3.0'],
    install: true,
    namespace: 'Gfls',
    nsversion: GFLS_API_VERSION,
    sources: [
      gfls_public_headers,
      gfls_public_c_files,
      gfls_enum_types,
    ],
    # Support for deps being built as subprojects:
    dependencies: GFLS_DEPS,
  )
endif
