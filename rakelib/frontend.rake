# Rakefile: frontend

FRONTEND_RAKE_SOURCES = FileList['rakelib/frontend.rake', 'Rakefile', 'rakelib/configuration.rake', 'rakelib/build_functions.rake']
FRONTEND_RAKE_CACHE   = "#{BUILD_PATH}/frontend/rake.cache"

FRONTEND_SOURCES      = FileList['frontend/**/*.cpp'].exclude('frontend/tests/**/*.cpp')

FRONTEND_OBJECTS      = objects_from_sources(FRONTEND_SOURCES, pch:COMPILER_PCH)

# Tasks

namespace(:compiler) do
  desc("Build the chime compiler commandline tool")
  task(:frontend => ["#{BUILD_PATH}/chime"])
end

# Rules

CLEAN.include("#{BUILD_PATH}/frontend")
CLEAN.include("#{BUILD_PATH}/chime")

# Build system cache
define_build_system_cache(FRONTEND_RAKE_SOURCES, FRONTEND_RAKE_CACHE)

file("#{BUILD_PATH}/chime" => "#{BUILD_PATH}/libchimecompiler.a")
file("#{BUILD_PATH}/chime" => FRONTEND_OBJECTS) do
  log("Link", "#{BUILD_PATH}/chime")
  sh("#{LINKER} -L#{BUILD_PATH} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES} -lchimecompiler -o #{BUILD_PATH}/chime #{FRONTEND_OBJECTS}")
end
