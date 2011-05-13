# Rakefile
require 'rake/clean'

BUILD_PATH = "/tmp/chime"
LLVM_PATH  = "/Users/matt/Documents/programming/build/Release+Asserts"

# this controls the verbosity of sh
verbose(false)

# basic tasks
directory BUILD_PATH

CLEAN.include("#{BUILD_PATH}/*")

# target tasks
task :default => ['compiler:all', 'runtime:all', 'library:all']

desc "Print out the build configuration variables"
task :print_config do
  puts("LLVM_PATH:      '#{LLVM_PATH}'")
  puts("LLVM_CONFIG:    '#{LLVM_CONFIG}'")
  puts("LLVM_CXX_FLAGS: '#{LLVM_CXX_FLAGS}'")
  puts("LLVM_LD_FLAGS:  '#{LLVM_LD_FLAGS}'")
  puts("LLVM_LIBRARIES: '#{LLVM_LIBRARIES}'")
  puts("")
  puts("COMPILER_FLAGS: '#{COMPILER_FLAGS}'")
  puts("LINKER_FLAGS:   '#{LINKER_FLAGS}'")
  puts("CC:             '#{CC}'")
  puts("CXX:            '#{CXX}'")
  puts("LINKER:         '#{LINKER}'")
  puts("ARCHIVER:       '#{ARCHIVER}'")
  puts("")
  puts("BUILD_PATH:     '#{BUILD_PATH}'")
end

namespace :compiler do
  desc 'Build the main frontend binary, chime'
  task :frontend => ["#{BUILD_PATH}/chime"]
  
  desc 'Build the chime compiler library'
  task :library => ["#{BUILD_PATH}/libchimecompiler.a"]
  
  desc 'Run the compiler tests'
  task :test, [:filter] => ["#{BUILD_PATH}/chime_test"] do |task, arguments|
    filter = nil
    if arguments[:filter]
      filter = "--gtest_filter=#{arguments[:filter]}"
    end

    log("Execute", "#{BUILD_PATH}/chime_test #{filter}")
    sh "#{BUILD_PATH}/chime_test #{filter}"
  end
  
  task :all => [:frontend, :test]
end

namespace :runtime do
  desc "Build the runtime library"
  task :build => ["#{BUILD_PATH}/libchimeruntime.a"]
  
  desc "Run the runtime tests"
  task :test => ["#{BUILD_PATH}/runtime_test"] do
    log("Execute", "#{BUILD_PATH}/runtime_test")
    sh "#{BUILD_PATH}/runtime_test"
  end
  
  task :all => [:test]
end

namespace :library do
  desc "Build the core class library"
  task :build => ["#{BUILD_PATH}/libchime.a"]
  
  desc "Run the runtime tests"
  task :test => ["#{BUILD_PATH}/library_test"] do
    log("Execute", "#{BUILD_PATH}/library_test")
    sh "#{BUILD_PATH}/library_test"
  end
  
  task :all# => [:test]
end
