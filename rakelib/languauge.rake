
LANGUAGE_TEST_SOURCES = FileList['tests/language/**/*.chm']

# Tasks

namespace(:language) do
  desc("Run the language tests")
  task(:test => "#{BUILD_PATH}/language_test") do
    execute_test_binary("#{BUILD_PATH}/language_test")
  end
end

# Rules
CLEAN.include("#{BUILD_PATH}/language_test")
CLEAN.include('tests/language/**/*.chc')

# language tests
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/libchimeruntime.a")
file("#{BUILD_PATH}/language_test" => "#{BUILD_PATH}/chime")
file("#{BUILD_PATH}/language_test" => LANGUAGE_TEST_SOURCES) do
  log("Build", "#{BUILD_PATH}/language_test")
  sh("#{CHIME_COMPILER} -o #{BUILD_PATH}/language_test tests/language/TestRunner.chm")
end