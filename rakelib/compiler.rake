# Rakefile: compiler

COMPILER_RAKE_SOURCES  = FileList['rakelib/compiler.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
COMPILER_RAKE_CACHE    = "#{BUILD_PATH}/compiler/rake.cache"

# Compiler Source Definitions
COMPILER_SOURCES       = FileList['compiler/**/*.cpp'].exclude('compiler/tests/**/*.cpp')
COMPILER_PREFIX_HEADER = 'compiler/prefix.hpp'

COMPILER_PCH           = pch_from_header(COMPILER_PREFIX_HEADER, rake_cache:COMPILER_RAKE_CACHE)
COMPILER_OBJECTS       = objects_from_sources(COMPILER_SOURCES, pch:COMPILER_PCH, rake_cache:COMPILER_RAKE_CACHE)

# Compiler Test Source Definitions
COMPILER_TEST_SOURCES            = FileList['compiler/tests/**/*.cpp']
COMPILER_TEST_PREFIX_HEADER      = 'compiler/tests/prefix.hpp'
COMPILER_TEST_FIXTURES           = FileList['compiler/tests/fixtures/**/*.chm']

COMPILER_TEST_PCH                = pch_from_header(COMPILER_TEST_PREFIX_HEADER, rake_cache:COMPILER_RAKE_CACHE)
COMPILER_TEST_OBJECTS            = objects_from_sources(COMPILER_TEST_SOURCES, pch:COMPILER_TEST_PCH, rake_cache:COMPILER_RAKE_CACHE)
COMPILER_TEST_INSTALLED_FIXTURES = copied_files_from_sources(COMPILER_TEST_FIXTURES, rake_cache:COMPILER_RAKE_CACHE)

# Frontend Sorce Definitions
FRONTEND_SOURCES = FileList['compiler/frontend/**/*.cpp']
FRONTEND_OBJECTS = objects_from_sources(FRONTEND_SOURCES)

# Tasks
namespace(:compiler) do
  desc("Build the chime compiler library")
  task(:library => ["#{BUILD_PATH}/libchimecompiler.a"])
  
  desc("Run the compiler tests, with an optional filter parameter")
  task(:test, [:filter] => ["#{BUILD_PATH}/chime_test"]) do |task, arguments|
    execute_test_binary("#{BUILD_PATH}/chime_test", arguments[:filter])
  end
  
  desc("Build the chime compiler commandline tool")
  task(:frontend => ["#{BUILD_PATH}/chime", "#{BUILD_PATH}/libchime.a", "#{BUILD_PATH}/libchimeruntime.a"])
end

# Rules

CLEAN.include("#{BUILD_PATH}/compiler")
CLEAN.include("#{BUILD_PATH}/libchimecompiler.a")
CLEAN.include("#{BUILD_PATH}/chime_test")
CLEAN.include("#{BUILD_PATH}/frontend")
CLEAN.include("#{BUILD_PATH}/chime")
CLEAN.include(COMPILER_PCH)
CLEAN.include(COMPILER_TEST_PCH)

# Build system cache
define_build_system_cache(COMPILER_RAKE_SOURCES, COMPILER_RAKE_CACHE)

# compiler library
file("#{BUILD_PATH}/libchimecompiler.a" => COMPILER_RAKE_CACHE)
file("#{BUILD_PATH}/libchimecompiler.a" => COMPILER_OBJECTS)
file("#{BUILD_PATH}/libchimecompiler.a") do
  library(COMPILER_OBJECTS, "#{BUILD_PATH}/libchimecompiler.a")
end

# compiler test binary
file("#{BUILD_PATH}/chime_test" => COMPILER_RAKE_CACHE)
file("#{BUILD_PATH}/chime_test" => :gtest)
file("#{BUILD_PATH}/chime_test" => COMPILER_TEST_INSTALLED_FIXTURES)
file("#{BUILD_PATH}/chime_test" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime_test" => COMPILER_TEST_OBJECTS)
file("#{BUILD_PATH}/chime_test") do
  log("Link", "#{BUILD_PATH}/chime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime_test #{COMPILER_TEST_OBJECTS}")
end

# frontend
file("#{BUILD_PATH}/chime" => COMPILER_RAKE_CACHE)
file("#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime" => FRONTEND_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime")
  sh("#{LINKER} -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
end
