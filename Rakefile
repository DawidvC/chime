# Rakefile
require 'rake/clean'

BUILD_PATH = "/tmp/chime"

# basic tasks
directory BUILD_PATH

CLEAN.include("#{BUILD_PATH}/*")

# target tasks
task :default => :all

task :all => [:frontend, :test]

desc 'Build the chime library'
task :library => ["#{BUILD_PATH}/libchime.a"]

desc 'Run the test binary'
task :test, [:filter] => ["#{BUILD_PATH}/chime_test"] do |task, arguments|
  filter = nil
  if arguments[:filter]
    filter = "--gtest_filter=#{arguments[:filter]}"
  end
  
  log("Execute", "#{BUILD_PATH}/chime_test #{filter}")
  sh "#{BUILD_PATH}/chime_test #{filter}"
end

desc "Print out the build configuration variables"
task :print_config do
  puts("LLVM_PATH:      '#{LLVM_PATH}'")
  puts("LLVM_CONFIG:    '#{LLVM_CONFIG}'")
  puts("LLVM_CXX_FLAGS: '#{LLVM_CXX_FLAGS}'")
  puts("LLVM_LD_FLAGS:  '#{LLVM_LD_FLAGS}'")
  puts("LLVM_LIBRARIES: '#{LLVM_LIBRARIES}'")
  puts("")
  puts("CXX_FLAGS:      '#{CXX_FLAGS}'")
  puts("LINKER_FLAGS:   '#{LINKER_FLAGS}'")
  puts("CXX:            '#{CXX}'")
  puts("LINKER:         '#{LINKER}'")
  puts("")
  puts("BUILD_PATH:     '#{BUILD_PATH}'")
end

desc 'Build the main frontend binary, chime'
task :frontend => ["#{BUILD_PATH}/chime"]
