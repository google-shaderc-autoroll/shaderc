use_relative_paths = True

vars = {
  'google_git':  'https://github.com/google',
  'khronos_git': 'https://github.com/KhronosGroup',

  'effcee_revision' : '8f0a61dc95e0df18c18e0ac56d83b3fa9d2fe90b',
  'glslang_revision': 'f88e5824d2cfca5edc58c7c2101ec9a4ec36afac',
  'googletest_revision': '0599a7b8410dc5cfdb477900b280475ae775d7f9',
  're2_revision': '90970542fe952602f42150c6e71d086f5afebcb3',
  'spirv_headers_revision': 'c4f8f65792d4bf2657ca751904c511bbcf2ac77b',
  'spirv_tools_revision': '0125b28ed4214d1860696f22d230dbfc965c6c2c',
  'spirv_cross_revision': 'fce83b7e8b0f6599efd4481992b2eb30f69f21de',
}

deps = {
  'third_party/effcee': Var('google_git') + '/effcee.git@' +
      Var('effcee_revision'),

  'third_party/googletest': Var('google_git') + '/googletest.git@' +
      Var('googletest_revision'),

  'third_party/glslang': Var('khronos_git') + '/glslang.git@' +
      Var('glslang_revision'),

  'third_party/re2': Var('google_git') + '/re2.git@' +
      Var('re2_revision'),

  'third_party/spirv-headers': Var('khronos_git') + '/SPIRV-Headers.git@' +
      Var('spirv_headers_revision'),

  'third_party/spirv-tools': Var('khronos_git') + '/SPIRV-Tools.git@' +
      Var('spirv_tools_revision'),

  'third_party/spirv-cross': Var('khronos_git') + '/SPIRV-Cross.git@' +
      Var('spirv_cross_revision'),
}
