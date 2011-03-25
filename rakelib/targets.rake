# Rakefile: targets

# test fixtures
directory "#{BUILD_PATH}/tests/fixtures"

task :test_fixtures => "#{BUILD_PATH}/tests/fixtures" do
  log("Copy", "#{BUILD_PATH}/tests/fixtures")
  verbose(false) do
    sh("cp -r tests/files/ #{BUILD_PATH}/tests/fixtures/")
  end
end

# test binary
file "#{BUILD_PATH}/chime_test" => :test_fixtures
file "#{BUILD_PATH}/chime_test" => "#{BUILD_PATH}/libchime.a"
file "#{BUILD_PATH}/chime_test" => TEST_OBJECTS do
  log("Link", "#{BUILD_PATH}/chime_test")
  verbose(false) do
    sh("#{LINKER} -lgtest -lgtest_main -L#{BUILD_PATH} -lchime -o #{BUILD_PATH}/chime_test #{TEST_OBJECTS}")
  end
end

# library
file "#{BUILD_PATH}/libchime.a" => LIBRARY_OBJECTS do
  log("Library", "#{BUILD_PATH}/libchime.a")
  verbose(false) do
    sh("ar rs #{BUILD_PATH}/libchime.a #{LIBRARY_OBJECTS}")
  end
end

# frontend
file "#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchime.a"
file "#{BUILD_PATH}/chime" => FRONTEND_OBJECTS do
  log("Link", "#{BUILD_PATH}/chime")
  verbose(false) do
    sh("#{LINKER} -L#{BUILD_PATH} -lchime -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
  end
end
