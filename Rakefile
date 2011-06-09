# Rakefile
require 'rake/clean'

BUILD_PATH = "/tmp/chime"
LLVM_PATH  = "/Users/matt/Documents/programming/build/Release+Asserts"

# this controls the verbosity of sh
verbose(false)

# basic tasks
directory(BUILD_PATH)

CLEAN.include("#{BUILD_PATH}/*")

# target tasks
task(:default => ['compiler:all', 'runtime:all', 'library:build', 'language:test'])

desc("Print the build configuration variables")
task(:print_config) do
  puts("LLVM_PATH:          '#{LLVM_PATH}'")
  puts("LLVM_CONFIG:        '#{LLVM_CONFIG}'")
  puts("LLVM_CXX_FLAGS:     '#{LLVM_CXX_FLAGS}'")
  puts("LLVM_LD_FLAGS:      '#{LLVM_LD_FLAGS}'")
  puts("LLVM_LIBRARIES:     '#{LLVM_LIBRARIES}'")
  puts("")
  puts("COMPILER_FLAGS:     '#{COMPILER_FLAGS}'")
  puts("LINKER_FLAGS:       '#{LINKER_FLAGS}'")
  puts("CC:                 '#{CC}'")
  puts("CXX:                '#{CXX}'")
  puts("LINKER:             '#{LINKER}'")
  puts("ARCHIVER:           '#{ARCHIVER}'")
  puts("")
  puts("BUILD_PATH:         '#{BUILD_PATH}'")
  puts("")
  puts("CHIME_COMPILER:     '#{CHIME_COMPILER}'")
  puts("CHIME_LINKER:       '#{CHIME_LINKER}'")
  puts("CHIME_LIBRARIES:    '#{CHIME_LIBRARIES}'")
  puts("CHIME_LINKER_FLAGS: '#{CHIME_LINKER_FLAGS}'")
end

namespace(:compiler) do
  desc("Build the main frontend binary, chime")
  task(:frontend => ["#{BUILD_PATH}/chime"])
  
  desc("Build the chime compiler library")
  task(:library => ["#{BUILD_PATH}/libchimecompiler.a"])
  
  desc("Run the compiler tests, with an optional filter parameter")
  task(:test, [:filter] => ["#{BUILD_PATH}/chime_test"]) do |task, arguments|
    execute_test_binary("#{BUILD_PATH}/chime_test", arguments[:filter])
  end
  
  # the explicit namespacing seems necessary for multitask for some
  # weird reason.  This could be a bug in rake.
  task(:all => ['compiler:test', 'compiler:frontend'])
end

namespace(:runtime) do
  desc("Build the runtime library")
  task(:library => ["#{BUILD_PATH}/libchimeruntime.a"])
  
  desc("Run the runtime tests")
  task(:test => ["#{BUILD_PATH}/runtime_test"]) do
    execute_test_binary("#{BUILD_PATH}/runtime_test")
  end
  
  task(:all => :test)
end

namespace(:library) do
  desc("Build the core class library")
  task(:build => ["#{BUILD_PATH}/libchime.a"])
  
  desc("Run the runtime tests")
  task(:test => ["#{BUILD_PATH}/library_test"]) do
    execute_test_binary("#{BUILD_PATH}/library_test")
  end
end

namespace(:language) do
  desc("Run the language tests")
  task(:test => ["#{BUILD_PATH}/language_test"]) do
    execute_test_binary("#{BUILD_PATH}/language_test")
  end
end
