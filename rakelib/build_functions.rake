# Rakefile: build functions and helpers

# helper functions
class String
  def red
    "\e[31m#{self}\e[0m"
  end
  def bold
    "\e[1m#{self}\e[0m"
  end
end

def log(key, message)
  length = 7 - key.length
  length = key.length if length < 0
  
  filler = ' ' * length
  
  puts(filler + "[#{key}]".bold + ": " + message)
end

# functions for building/executing
def compiler_for(source_file)
  case File.extname(source_file)
  when ".cpp", ".cc", ".hpp"
    CXX
  when ".c", ".h"
    CC
  when ".chm", ".chime"
    CHIME_COMPILER
  else
    raise("Don't know how to compile #{source_file}")
  end
end

def compile(source_file, object_file, options={})
  log("Compile", source_file)
  
  cmd  = compiler_for(source_file)
  cmd += " -c"
  cmd += " -o #{object_file}"
  
  if options[:pch]
    header = options[:pch][0...-4] # remove the ".pch" extension
    cmd += " -include #{header}"
  end
  
  cmd += " #{source_file}"
  
  sh(cmd, :verbose => false)
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
def define_build_system_cache(filelist, cache_file)
  dir = File.dirname(cache_file)
  
  directory(dir)
  
  file(cache_file => dir)
  file(cache_file => filelist) do
    log("Touch", cache_file)
    sh("touch #{cache_file}", :verbose => false)
  end
end

def define_dependencies_for_source(source_file, options={})
  options[:rake_cache] ||= RAKE_FILES
  
  full_path = File.join(Rake.original_dir, source_file)
  
  object_file = case File.extname(source_file)
  when '.h', '.hpp'
    source_file + '.gch'
  when '.c', '.cc', '.cpp'
    BUILD_PATH + '/' + source_file.ext('o')
  else
    raise("Not sure how to define dependencies for #{source_file}")
  end
  
  dependency_cache_file = BUILD_PATH + '/' + source_file.ext('dependencies.cache')
  object_dir            = File.dirname(object_file)
  
  raise("Could not find source file #{source_file}") unless File.exist?(source_file)
  
  # ensure we have a directory
  directory(object_dir)
  
  cached_dependencies = []
  if File.exist?(dependency_cache_file)
    File.open(dependency_cache_file, "r") { |f| cached_dependencies = f.read().split() }
    
    # check to make sure all cached files all still exist!
    cached_dependencies.clear() if cached_dependencies.any? { |dep| !File.exist?(dep) }
  end
  
  file(dependency_cache_file => object_dir)
  file(dependency_cache_file => options[:rake_cache])
  file(dependency_cache_file => options[:pch]) if options[:pch]
  file(dependency_cache_file => cached_dependencies)
  file(dependency_cache_file) do
    log("Headers", source_file)
    string = `#{compiler_for(full_path)} -MM #{full_path}`.chomp()
    
    cached_dependencies = string.split(" \\\n  ")
    cached_dependencies = cached_dependencies[1, cached_dependencies.length]
    
    cached_dependencies.collect! { |x| File.absolute_path(x) }
    
    File.open(dependency_cache_file, "w+") { |f| f.puts(cached_dependencies.join("\n")) }
  end
  
  file(object_file => [object_dir, dependency_cache_file, source_file]) do
    compile(source_file, object_file, options)
  end
  
  object_file
end

def objects_from_sources(source_filelist, options={})
  source_filelist.collect { |x| define_dependencies_for_source(x, options) }
end

def pch_from_header(header, options={})
  define_dependencies_for_source(header, options)
end

def copied_files_from_sources(filelist, options={})
  raise if filelist.empty?
  
  filelist.collect do |fixture_source|
    installed_fixture = BUILD_PATH + '/' + fixture_source
    
    dir = File.dirname(installed_fixture)
    
    directory(dir)
    
    file(installed_fixture => [dir, fixture_source]) do
      log("Copy", fixture_source)
      
      sh("cp -r #{fixture_source} #{installed_fixture}", :verbose => false)
    end
    
    installed_fixture
  end
end
