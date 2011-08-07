# Rakefile
require 'rake/clean'

BUILD_PATH = "/tmp/chime"
LLVM_PATH  = "/usr/local"
GTEST_PATH = "/usr/local/gtest-1.6.0"

# this controls the verbosity of sh
verbose(false)

RAKE_FILES = FileList['Rakefile', 'rakelib/**/*.rake']

# target tasks
desc("Run all tests for all targets")
task(:default => ['compiler:test', 'runtime:test', 'library:test', 'language:test'])

desc("Print the build configuration variables")
task(:print_config) do
  puts("GTEST_PATH:         '#{GTEST_PATH}'")
  puts("")
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
  puts("CHIME_LIBRARIES:    '#{CHIME_LIBRARIES}'")
end
