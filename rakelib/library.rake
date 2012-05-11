# Rakefile: library

LIBRARY_RAKE_SOURCES  = FileList['rakelib/library.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
LIBRARY_RAKE_CACHE    = "#{BUILD_PATH}/library/rake.cache"

# Library Source Definitions
LIBRARY_SOURCES       = FileList['library/**/*.c'].exclude('library/Tests/**/*')
LIBRARY_CHIME_SOURCES = FileList['library/**/*.cm'].exclude('library/Tests/**/*')

LIBRARY_OBJECTS       = objects_from_sources(LIBRARY_SOURCES, rake_cache:LIBRARY_RAKE_CACHE)

# Library Test Definitions
LIBRARY_TEST_SOURCES  = FileList['library/Tests/**/*.cm']
LIBRARY_TEST_BINARY   = "#{BUILD_PATH}/library_test"

# Tasks

namespace(:library) do
  desc("Build the core class library")
  task(:build => ["#{BUILD_PATH}/libchime.a"])
  
  desc("Run the library tests")
  task(:test => LIBRARY_TEST_BINARY) do
    execute_test_binary(LIBRARY_TEST_BINARY)
  end
end

# Rules

CLEAN.include("#{BUILD_PATH}/library")
CLEAN.include("#{BUILD_PATH}/libchime.a")
CLEAN.include("library/**/*.chc")

# Build system cache
define_build_system_cache(LIBRARY_RAKE_SOURCES, LIBRARY_RAKE_CACHE)

# Standard Library
file("#{BUILD_PATH}/libchime.a" => LIBRARY_RAKE_CACHE)
file("#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS)
file("#{BUILD_PATH}/libchime.a" => LIBRARY_CHIME_SOURCES)
file("#{BUILD_PATH}/libchime.a" => "#{BUILD_PATH}/chime") # feels almost circular
file("#{BUILD_PATH}/libchime.a") do
  sh("#{CHIME_COMPILER} -t -l -c library/chime_based_library.cm")
  
  LIBRARY_CHIME_OBJECTS = LIBRARY_CHIME_SOURCES.ext(".chc")
  
  objects = LIBRARY_OBJECTS + LIBRARY_CHIME_OBJECTS
  
  library(objects, "#{BUILD_PATH}/libchime.a")
end

# Library Tests
file("/usr/local/lib/libchime.a")
file("/usr/local/lib/libchimeruntime.a")

file(LIBRARY_TEST_BINARY => "/usr/local/lib/libchime.a")
file(LIBRARY_TEST_BINARY => "/usr/local/lib/libchimeruntime.a")
file(LIBRARY_TEST_BINARY => "#{BUILD_PATH}/libchime.a")
file(LIBRARY_TEST_BINARY => "#{BUILD_PATH}/libchimeruntime.a")
file(LIBRARY_TEST_BINARY => "#{BUILD_PATH}/chime")
file(LIBRARY_TEST_BINARY => LIBRARY_TEST_SOURCES) do
  log("Build", LIBRARY_TEST_BINARY)
  sh("#{CHIME_COMPILER} -c -o #{LIBRARY_TEST_BINARY} library/Tests/Tests.cm")
end
