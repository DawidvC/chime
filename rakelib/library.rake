# Rakefile: library

LIBRARY_RAKE_SOURCES = FileList['rakelib/library.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
LIBRARY_RAKE_CACHE   = "#{BUILD_PATH}/library/rake.cache"

# Library Source Definitions
LIBRARY_SOURCES      = FileList['library/**/*.c'].exclude('library/tests/**/*.cpp')

LIBRARY_OBJECTS      = objects_from_sources(LIBRARY_SOURCES, rake_cache:LIBRARY_RAKE_CACHE)

# Tasks

namespace(:library) do
  desc("Build the core class library")
  task(:build => ["#{BUILD_PATH}/libchime.a"])
  
  desc("Run the library tests (Unimplemented)")
  task(:test => :build)
end

# Rules

CLEAN.include("#{BUILD_PATH}/library")
CLEAN.include("#{BUILD_PATH}/libchime.a")

# Build system cache
define_build_system_cache(LIBRARY_RAKE_SOURCES, LIBRARY_RAKE_CACHE)

# Standard Library
file("#{BUILD_PATH}/libchime.a" => LIBRARY_RAKE_CACHE)
file("#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS)
file("#{BUILD_PATH}/libchime.a") do
  library(LIBRARY_OBJECTS, "#{BUILD_PATH}/libchime.a")
end