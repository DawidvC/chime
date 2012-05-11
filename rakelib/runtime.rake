# Rakefile: runtime

RUNTIME_RAKE_SOURCES = FileList['rakelib/runtime.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
RUNTIME_RAKE_CACHE   = "#{BUILD_PATH}/runtime/rake.cache"

# Runtime Source Definitions
RUNTIME_SOURCES      = FileList['runtime/**/*.c'].exclude('runtime/tests/**/*.cpp')

RUNTIME_OBJECTS      = objects_from_sources(RUNTIME_SOURCES, rake_cache:RUNTIME_RAKE_CACHE)

# Runtime Test Source Definitions
RUNTIME_TEST_SOURCES       = FileList['runtime/tests/**/*.cpp']
RUNTIME_TEST_PREFIX_HEADER = 'runtime/tests/prefix.hpp'

RUNTIME_TEST_PCH           = pch_from_header(RUNTIME_TEST_PREFIX_HEADER, rake_cache:RUNTIME_RAKE_CACHE)
RUNTIME_TEST_OBJECTS       = objects_from_sources(RUNTIME_TEST_SOURCES, pch:RUNTIME_TEST_PCH, rake_cache:RUNTIME_RAKE_CACHE)

# Tasks
namespace(:runtime) do
  desc("Build the runtime library")
  task(:library => ["#{BUILD_PATH}/libchimeruntime.a"])
  
  desc("Run the runtime tests")
  task(:test => ["#{BUILD_PATH}/runtime_test"]) do
    execute_test_binary("#{BUILD_PATH}/runtime_test")
  end
end

# Rules

CLEAN.include("#{BUILD_PATH}/runtime")
CLEAN.include("#{BUILD_PATH}/libchimeruntime.a")
CLEAN.include("#{BUILD_PATH}/runtime_test")
CLEAN.include(RUNTIME_TEST_PCH)

# Build system cache
define_build_system_cache(RUNTIME_RAKE_SOURCES, RUNTIME_RAKE_CACHE)

# Runtime Library
file("#{BUILD_PATH}/libchimeruntime.a" => RUNTIME_RAKE_CACHE)
file("#{BUILD_PATH}/libchimeruntime.a" => RUNTIME_OBJECTS)
file("#{BUILD_PATH}/libchimeruntime.a") do
  library(RUNTIME_OBJECTS, "#{BUILD_PATH}/libchimeruntime.a")
end

# Runtime test
file("#{BUILD_PATH}/runtime_test" => :gtest)
file("#{BUILD_PATH}/runtime_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/runtime_test" => RUNTIME_TEST_OBJECTS) do
  log("Link", "#{BUILD_PATH}/runtime_test")
  # Again, ubuntu linker is very sensitive here, and requires -lpthread
  sh("#{LINKER} -o #{BUILD_PATH}/runtime_test #{RUNTIME_TEST_OBJECTS} -L#{BUILD_PATH} -lchimeruntime -lgtest -lgtest_main -lpthread")
end
