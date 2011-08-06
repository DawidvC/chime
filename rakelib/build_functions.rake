# Rakefile: build functions and helpers

# functions for building/executing
def compiler_for(source_file)
  case File.extname(source_file)
  when ".cpp", ".cc"
    CXX
  when ".c"
    CC
  when ".chm", ".chime"
    CHIME_COMPILER
  else
    raise("Don't know how to compile #{source_file}")
  end
end

def compile(source_file, object_file)
  log("Compile", source_file.ext('o'))
  
  sh("#{compiler_for(source_file)} -c -o #{object_file} #{source_file}", :verbose => false)
end

def library(object_files, library_file)
  # make absolutely sure to rm the file, as it seems that ar
  # is not blindly rebuilding if the .a is still present
  rm_f(library_file)
  
  log("Library", library_file)
  sh("#{ARCHIVER} #{library_file} #{object_files}", :verbose => false)
end

def execute_test_binary(path, filter=nil)
  filter = "--gtest_filter=#{filter}" if filter
  
  log("Execute", "#{path} #{filter}")
  value = sh("#{path} #{filter}")
  
  result = value ? "Passed" : "Failed".red
  
  log("Test", "#{path}: #{result}")
end

# functions for dependency calculation
def define_dependencies_for_source(source_file)
  full_path         = File.join(Rake.original_dir, source_file)
  object_file       = BUILD_PATH + '/' + source_file.ext('o')
  dependencies_file = BUILD_PATH + '/' + source_file.ext('dependency_cache.yml')
  object_dir        = File.dirname(object_file)
  
  raise unless File.exist?(source_file)
  
  # ensure we have a directory
  directory(object_dir)
  
  cached_dependencies = []
  if File.exist?(dependencies_file)
    File.open(dependencies_file, "r") { |f| cached_dependencies = f.read().split() }
  end
  
  file(dependencies_file => object_dir)
  file(dependencies_file => BUILD_SYSTEM_CACHE)
  file(dependencies_file => cached_dependencies) do
    log("Headers", source_file)
    string = `#{compiler_for(full_path)} -MM #{full_path}`.chomp()
    
    cached_dependencies = string.split(" \\\n  ")
    cached_dependencies = cached_dependencies[2, cached_dependencies.length]
    
    cached_dependencies.collect! { |x| File.absolute_path(x) }
    
    cached_dependencies.sort!()
    
    File.open(dependencies_file, "w+") { |f| f.puts(cached_dependencies.join("\n")) }
  end
  
  file(object_file => [object_dir, source_file, dependencies_file]) do
    compile(source_file, object_file)
  end
  
  object_file
end

def objects_from_sources(source_filelist)
  source_filelist.collect { |x| define_dependencies_for_source(x) }
end
