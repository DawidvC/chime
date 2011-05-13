# Rakefile: configuration

# LLVM stuff
LLVM_CONFIG    = "#{LLVM_PATH}/bin/llvm-config"

output         = `#{LLVM_CONFIG} --cppflags --ldflags --libs core`.split("\n")
LLVM_CXX_FLAGS = output[0]
LLVM_LD_FLAGS  = output[1]
LLVM_LIBRARIES = output[2]

# Base stuff
COMPILER_FLAGS = "-Wall -I/opt/local/include -I#{Rake.original_dir}"
LINKER_FLAGS   = "-L/opt/local/lib"

# core compiler/linker stuff
CC             = "#{LLVM_PATH}/bin/clang #{COMPILER_FLAGS}"
CXX            = "#{LLVM_PATH}/bin/clang++ #{COMPILER_FLAGS} #{LLVM_CXX_FLAGS}"
LINKER         = "#{LLVM_PATH}/bin/clang++ #{LINKER_FLAGS} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES}"

# -r : add object files to the archive
# -s : add object-file index
ARCHIVER       = "ar rs"