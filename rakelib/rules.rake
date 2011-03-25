# Rakefile: rules

# make filelists for all of our sources
LIBRARY_SOURCES  = FileList['ast/**/*.cpp', 'lexer/**/*.cpp', 'parser/**/*.cpp']
TEST_SOURCES     = FileList['tests/**/*.cpp']
FRONTEND_SOURCES = FileList['frontend/**/*.cpp']

def dependencies_for_filelist(list)
  object_files = FileList.new()
  
  list.each do |source_file|
    
    object_file = BUILD_PATH + '/' + source_file.ext('o')
    header_file = source_file.ext('h')
    object_dir  = File.dirname(object_file)
    
    dependencies = Array.new()
    
    dependencies << source_file
    dependencies << header_file if File.exist?(File.join(Rake.original_dir,header_file))
    dependencies << object_dir
    
    # the object file depends on the path, source and header
    directory object_dir
    
    file object_file => dependencies do
      log("Compile", source_file.ext('o'))
      sh("#{CXX} -c #{source_file} -o #{object_file}")
    end
    
    object_files.add(object_file)
  end
  
  object_files
end

# create dependenices on the sources for each object file
LIBRARY_OBJECTS  = dependencies_for_filelist(LIBRARY_SOURCES)
TEST_OBJECTS     = dependencies_for_filelist(TEST_SOURCES)
FRONTEND_OBJECTS = dependencies_for_filelist(FRONTEND_SOURCES)

# make manual dependencies
