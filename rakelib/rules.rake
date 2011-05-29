# Rakefile: rules

# make filelists for all of our sources
COMPILER_SOURCES     = FileList['ast/**/*.cpp', 'lexer/**/*.cpp', 'parser/**/*.cpp', 'codegen/**/*.cpp']
TEST_SOURCES         = FileList['tests/lexer/**/*.cpp', 'tests/parser/**/*.cpp']
FRONTEND_SOURCES     = FileList['frontend/**/*.cpp']
RUNTIME_SOURCES      = FileList['runtime/**/*.c']
RUNTIME_TEST_SOURCES = FileList['tests/runtime/**/*.cpp']
LIBRARY_SOURCES      = FileList['library/**/*.cpp']
LIBRARY_TEST_SOURCES = FileList['tests/library/**/*.cpp']
RAKE_SOURCES         = FileList['Rakefile', 'rakelib/*.rake']

def dependencies_for_filelist(list)
  object_files = FileList.new()
  
  list.each do |source_file|
    
    object_file = BUILD_PATH + '/' + source_file.ext('o')
    header_file = source_file.ext('h')
    object_dir  = File.dirname(object_file)
    
    dependencies = Array.new()
    
    dependencies << "#{BUILD_PATH}/rake_cache"
    dependencies << source_file
    dependencies << object_dir
    
    # add the main header, and also scan it for additional dependencies
    header_file_path = File.join(Rake.original_dir, header_file)
    if File.exist?(header_file_path)
      dependencies << header_file
      
      dependencies += header_dependencies_for_file(header_file_path)
    end
    
    # deal with the detected headers
    dependencies += header_dependencies_for_file(source_file)
    
    # the object file depends on the path, source and header
    directory(object_dir)
    
    file(object_file => dependencies) do
      compile(source_file, object_file)
    end
    
    object_files.add(object_file)
  end
  
  object_files
end

# create dependenices on the sources for each object file
COMPILER_OBJECTS     = dependencies_for_filelist(COMPILER_SOURCES)
TEST_OBJECTS         = dependencies_for_filelist(TEST_SOURCES)
FRONTEND_OBJECTS     = dependencies_for_filelist(FRONTEND_SOURCES)
RUNTIME_OBJECTS      = dependencies_for_filelist(RUNTIME_SOURCES)
RUNTIME_TEST_OBJECTS = dependencies_for_filelist(RUNTIME_TEST_SOURCES)
LIBRARY_OBJECTS      = dependencies_for_filelist(LIBRARY_SOURCES)
LIBRARY_TEST_OBJECTS = dependencies_for_filelist(LIBRARY_TEST_SOURCES)

