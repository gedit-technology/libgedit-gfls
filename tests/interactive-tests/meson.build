executable(
  'gfls-program',
  'gfls-program.c',
  dependencies: GFLS_DEPS,
)

executable(
  'gfls-test-race-read-content',
  'gfls-test-race-read-content.c',
  dependencies: GFLS_DEPS,
)

if GTK3_DEP.found()
  executable(
    'gfls-gui',
    'gfls-gui.c',
    dependencies: [GFLS_LIB_DEP, GTK3_DEP],
  )
endif
