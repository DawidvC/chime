# Rakefile: build functions and helpers

# functions for building/executing
def compile(source_file, object_file)
  log("Compile", source_file.ext('o'))
  
  compiler = case File.extname(source_file)
  when ".cpp"
    CXX
  when ".c"
    CC
  when ".chm", ".chime"
    CHIME_COMPILER
  else
    raise("Don't know how to compile #{path}")
  end
  
  sh("#{compiler} -c -o #{object_file} #{source_file}", :verbose => false)
end

def execute_test_binary(path, filter=nil)
  filter = "--gtest_filter=#{filter}" if filter
  
  log("Execute", "#{path} #{filter}")
  value = sh("#{path} #{filter}")
  
  result = value ? "Passed" : "Failed".red
  
  log("Test", "#{path}: #{result}")
end

# functions for dependency calculation
def header_dependencies_for_file(path)
  return unless File.exist?(path)
  
  includes = Array.new()
  
  File.open(path) do |file|
    until file.eof?
      line = file.readline()
      
      case line[0,6]
      when "class "
        break
      when "namesp"
        break
      when "#inclu"
        project_header = line[/#include "([\/\w]+\.h)"/, 1]
        
        # must not be nil and must exist
        next if project_header.nil?
        next unless File.exist?(File.join(Rake.original_dir, project_header))
        
        includes << project_header
      end
    end
  end
  
  includes
end
