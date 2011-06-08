# Rakefile: targets

# the build system itself
file("#{BUILD_PATH}/rake_cache" => LLVM_CONFIG)
file("#{BUILD_PATH}/rake_cache" => RAKE_SOURCES) do
  log("Touch", "rake_cache")
  sh("touch #{BUILD_PATH}/rake_cache", :verbose => false)
end

# test fixtures
directory("#{BUILD_PATH}/tests/fixtures")

task(:test_fixtures => "#{BUILD_PATH}/tests/fixtures") do
  log("Copy", "#{BUILD_PATH}/tests/fixtures")
  sh("cp -r tests/fixtures/ #{BUILD_PATH}/tests/fixtures/", :verbose => false)
end

# compiler test binary
file("#{BUILD_PATH}/chime_test" => :test_fixtures)
file("#{BUILD_PATH}/chime_test" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime_test" => TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime_test #{TEST_OBJECTS}")
end

# compiler library
file("#{BUILD_PATH}/libchimecompiler.a" => COMPILER_OBJECTS) do
  log("Library", "#{BUILD_PATH}/libchimecompiler.a")
  # -r : add object files to the archive
  # -s : add object-file index
  sh("#{ARCHIVER} #{BUILD_PATH}/libchimecompiler.a #{COMPILER_OBJECTS}", :verbose => false)
end

# frontend
file("#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime" => FRONTEND_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime")
  sh("#{LINKER} -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
end

# runtime
file("#{BUILD_PATH}/libchimeruntime.a" => RUNTIME_OBJECTS) do
  log("Library", "#{BUILD_PATH}/libchimeruntime.a")
  sh("#{ARCHIVER} #{BUILD_PATH}/libchimeruntime.a #{RUNTIME_OBJECTS}", :verbose => false)
end

# runtime test binary
file("#{BUILD_PATH}/runtime_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/runtime_test" => RUNTIME_TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/runtime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} -lchimeruntime -o #{BUILD_PATH}/runtime_test #{RUNTIME_TEST_OBJECTS}")
end

# library
file("#{BUILD_PATH}/libchime.a" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS) do
  log("Library", "#{BUILD_PATH}/libchime.a")
  sh("#{ARCHIVER} #{BUILD_PATH}/libchime.a #{LIBRARY_OBJECTS}", :verbose => false)
end

# language tests
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/chime")
file("#{BUILD_PATH}/language_test" => LANGUAGE_TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/language_test")
  sh("#{CHIME_LINKER} -o #{BUILD_PATH}/language_test #{LANGUAGE_TEST_OBJECTS}", :verbose => false)
end
