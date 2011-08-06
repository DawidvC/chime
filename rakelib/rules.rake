# Rakefile: rules

# make filelists for all of our sources
RAKE_SOURCES          = FileList['Rakefile', 'rakelib/*.rake']
COMPILER_SOURCES      = FileList['ast/**/*.cpp', 'lexer/**/*.cpp', 'parser/**/*.cpp', 'codegen/**/*.cpp']
TEST_SOURCES          = FileList['tests/lexer/**/*.cpp', 'tests/parser/**/*.cpp']
TEST_FIXTURES         = FileList['tests/fixtures/**/*']
FRONTEND_SOURCES      = FileList['frontend/**/*.cpp']
RUNTIME_SOURCES       = FileList['runtime/**/*.c']
RUNTIME_TEST_SOURCES  = FileList['tests/runtime/**/*.cpp']
LIBRARY_SOURCES       = FileList['library/**/*.c']
LANGUAGE_TEST_SOURCES = FileList['tests/language/**/*.chm']

# make some special objects
BUILD_SYSTEM_CACHE        = "#{BUILD_PATH}/build_system.cache"

# create fileslists for all objects based on the sources
COMPILER_OBJECTS      = objects_from_sources(COMPILER_SOURCES)
TEST_OBJECTS          = objects_from_sources(TEST_SOURCES)
FRONTEND_OBJECTS      = objects_from_sources(FRONTEND_SOURCES)
RUNTIME_OBJECTS       = objects_from_sources(RUNTIME_SOURCES)
RUNTIME_TEST_OBJECTS  = objects_from_sources(RUNTIME_TEST_SOURCES)
LIBRARY_OBJECTS       = objects_from_sources(LIBRARY_SOURCES)
