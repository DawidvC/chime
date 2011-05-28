def execute_test_binary(path, filter=nil)
  filter = "--gtest_filter=#{filter}" if filter
  
  log("Execute", "#{BUILD_PATH}/chime_test #{filter}")
  sh("#{path} #{filter}")
end
