# Rakefile: targets

# test fixtures
directory "#{BUILD_PATH}/tests/fixtures"

task :test_fixtures => "#{BUILD_PATH}/tests/fixtures" do
  log("Copy", "#{BUILD_PATH}/tests/fixtures")
  sh("cp -r tests/files/ #{BUILD_PATH}/tests/fixtures/")
end

# test binary
file "#{BUILD_PATH}/chime_test" => :test_fixtures
file "#{BUILD_PATH}/chime_test" => "#{BUILD_PATH}/libchime.a"
file "#{BUILD_PATH}/chime_test" => TEST_OBJECTS do
  log("Link", "#{BUILD_PATH}/chime_test")
  sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} -lchime -o #{BUILD_PATH}/chime_test #{TEST_OBJECTS}")
end

# library
file "#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS do
  log("Library", "#{BUILD_PATH}/libchime.a")
  # -r : add object files to the archive
  # -s : add object-file index
  sh("ar rs #{BUILD_PATH}/libchime.a #{LIBRARY_OBJECTS}")
end

# frontend
file "#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchime.a"
file "#{BUILD_PATH}/chime" => FRONTEND_OBJECTS do
  log("Link", "#{BUILD_PATH}/chime")
  sh("#{LINKER} -L#{BUILD_PATH} -lchime -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
end
