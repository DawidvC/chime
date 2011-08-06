# Rakefile: targets

# the build system itself
directory(BUILD_PATH)

file(BUILD_SYSTEM_CACHE => BUILD_PATH)
file(BUILD_SYSTEM_CACHE => LLVM_CONFIG)
file(BUILD_SYSTEM_CACHE => RAKE_SOURCES) do
  log("Touch", BUILD_SYSTEM_CACHE)
  sh("touch #{BUILD_SYSTEM_CACHE}", :verbose => false)
end

# clean task
CLEAN.include("#{BUILD_PATH}/*")

# gtest binaries
# You might be wondering why this heck this is done here.  Because the gtest libraries
# use templates and macros like crazy, it's possible that changes to the compiler
# configuration make for incompatible binaries.  The googletest team recommendation is to
# rebuild these every time you need them, so there you go.
directory("#{BUILD_PATH}/gtest")

file("#{BUILD_PATH}/libgtest.a" => "#{BUILD_PATH}/gtest")
file("#{BUILD_PATH}/libgtest.a" => BUILD_SYSTEM_CACHE) do
  compile("#{GTEST_PATH}/src/gtest-all.cc", "#{BUILD_PATH}/gtest/gtest-all.o")
  
  library("#{BUILD_PATH}/gtest/gtest-all.o", "#{BUILD_PATH}/libgtest.a")
end

file("#{BUILD_PATH}/libgtest_main.a" => "#{BUILD_PATH}/gtest")
file("#{BUILD_PATH}/libgtest_main.a" => BUILD_SYSTEM_CACHE) do
  compile("#{GTEST_PATH}/src/gtest_main.cc", "#{BUILD_PATH}/gtest/gtest_main.o")
  
  library("#{BUILD_PATH}/gtest/gtest_main.o", "#{BUILD_PATH}/libgtest_main.a")
end

task(:gtest => ["#{BUILD_PATH}/libgtest.a", "#{BUILD_PATH}/libgtest_main.a"])

# test fixtures

directory("#{BUILD_PATH}/tests/fixtures")

FIXTURE_CACHE = "#{BUILD_PATH}/tests/fixtures/fixtures.cache"
file(FIXTURE_CACHE => BUILD_SYSTEM_CACHE)
file(FIXTURE_CACHE => "#{BUILD_PATH}/tests/fixtures")
file(FIXTURE_CACHE => TEST_FIXTURES) do
  log("Copy", "#{BUILD_PATH}/tests/fixtures")
  sh("cp -r tests/fixtures/ #{BUILD_PATH}/tests/fixtures/", :verbose => false)
  
  log("Touch", FIXTURE_CACHE)
  sh("touch #{FIXTURE_CACHE}", :verbose => false)
end

task(:test_fixtures => FIXTURE_CACHE)

# compiler test binary
file("#{BUILD_PATH}/chime_test" => [:test_fixtures, :gtest])
file("#{BUILD_PATH}/chime_test" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime_test" => TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime_test #{TEST_OBJECTS}")
end

# compiler library
file("#{BUILD_PATH}/libchimecompiler.a" => COMPILER_OBJECTS) do
  library(COMPILER_OBJECTS, "#{BUILD_PATH}/libchimecompiler.a")
end

# frontend
file("#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime" => FRONTEND_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime")
  sh("#{LINKER} -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
end

# runtime
file("#{BUILD_PATH}/libchimeruntime.a" => RUNTIME_OBJECTS) do
  library(RUNTIME_OBJECTS, "#{BUILD_PATH}/libchimeruntime.a")
end

# runtime test binary
file("#{BUILD_PATH}/runtime_test" => :gtest)
file("#{BUILD_PATH}/runtime_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/runtime_test" => RUNTIME_TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/runtime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} -lchimeruntime -o #{BUILD_PATH}/runtime_test #{RUNTIME_TEST_OBJECTS}")
end

# library
file("#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS) do
  library(LIBRARY_OBJECTS, "#{BUILD_PATH}/libchime.a")
end

# language tests
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/chime")
file("#{BUILD_PATH}/language_test" => LANGUAGE_TEST_SOURCES) do
  log("Build", "#{BUILD_PATH}/language_test")
  sh("#{CHIME_COMPILER} -o #{BUILD_PATH}/language_test tests/language/TestRunner.chm")
end
