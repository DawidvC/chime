# Rakefile: gtest

# You might be wondering why this heck this is done here.  Because the gtest libraries
# use templates and macros like crazy, it's possible that changes to the compiler
# configuration make for incompatible binaries.  The googletest team recommendation is to
# rebuild these every time you need them, so there you go.

GTEST_RAKE_SOURCES = FileList['rakelib/gtest.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
GTEST_RAKE_CACHE   = "#{BUILD_PATH}/gtest/rake.cache"

# Tasks
desc("Build the googletest libraries")
task(:gtest => ["#{BUILD_PATH}/libgtest.a", "#{BUILD_PATH}/libgtest_main.a"])

# Rules

# Build system cache
define_build_system_cache(GTEST_RAKE_SOURCES, GTEST_RAKE_CACHE)

CLEAN.include("#{BUILD_PATH}/gtest")
CLEAN.include("#{BUILD_PATH}/libgtest.a")
CLEAN.include("#{BUILD_PATH}/libgtest_main.a")

# gtest binaries

file("#{BUILD_PATH}/libgtest.a" => GTEST_RAKE_CACHE)
file("#{BUILD_PATH}/libgtest.a" => "#{BUILD_PATH}/gtest")
file("#{BUILD_PATH}/libgtest.a") do
  compile("#{GTEST_PATH}/src/gtest-all.cc", "#{BUILD_PATH}/gtest/gtest-all.o")
  
  library("#{BUILD_PATH}/gtest/gtest-all.o", "#{BUILD_PATH}/libgtest.a")
end

file("#{BUILD_PATH}/libgtest_main.a" => GTEST_RAKE_CACHE)
file("#{BUILD_PATH}/libgtest_main.a" => "#{BUILD_PATH}/gtest")
file("#{BUILD_PATH}/libgtest_main.a") do
  compile("#{GTEST_PATH}/src/gtest_main.cc", "#{BUILD_PATH}/gtest/gtest_main.o")
  
  library("#{BUILD_PATH}/gtest/gtest_main.o", "#{BUILD_PATH}/libgtest_main.a")
end
