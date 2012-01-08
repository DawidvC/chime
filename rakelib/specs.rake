
LANGUAGE_TEST_SOURCES = FileList['specs/Unit/*.chm']
SCRIPT_TEST_SOURCES   = FileList['specs/Script/*.chm']

TEST_SCRIPTS_PATH     = "#{BUILD_PATH}/specs/Script"

SCRIPT_TEST_EXECUTABLES = SCRIPT_TEST_SOURCES.collect do |source_file|
  script_name       = File.basename(source_file, ".chm")
  script_executable = "#{TEST_SCRIPTS_PATH}/#{script_name}"
  
  directory(TEST_SCRIPTS_PATH)
  
  file(script_executable => TEST_SCRIPTS_PATH)
  file(script_executable => "#{BUILD_PATH}/libchime.a")
  file(script_executable => "#{BUILD_PATH}/libchimeruntime.a")
  file(script_executable => "#{BUILD_PATH}/chime")
  file(script_executable => source_file) do
    sh("#{CHIME_COMPILER} -c -o #{script_executable} #{source_file}")
  end
  
  script_executable
end

# Tasks

namespace(:specs) do
  desc("Run the language spec unit tests")
  task(:unit => "#{BUILD_PATH}/language_test") do
    execute_test_binary("#{BUILD_PATH}/language_test")
  end
  
  desc("Run the language spec script-based tests")
  task(:scripts => SCRIPT_TEST_EXECUTABLES) do
    require_relative "../specs/Script/script_tests.rb"
    log("Test", "specs/Script/script_tests.rb")
  end
  
  desc("Run all the language spec tests")
  task(:run => [:unit, :scripts])
end

# Rules
CLEAN.include("#{BUILD_PATH}/language_test")
CLEAN.include('specs/Unit/*.chc')

# language tests
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/chime")
file("#{BUILD_PATH}/language_test" => LANGUAGE_TEST_SOURCES) do
  log("Build", "#{BUILD_PATH}/language_test")
  sh("#{CHIME_COMPILER} -c -o #{BUILD_PATH}/language_test specs/Unit/Tests.chm")
end
