unit_tests = [
  'test-utf8',
]

foreach test_name : unit_tests
  test_exe = executable(
    test_name,
    test_name + '.c',
    dependencies: GFLS_LIB_DEP
  )
  test(test_name, test_exe)
endforeach
