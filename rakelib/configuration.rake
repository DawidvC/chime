# Rakefile: configuration

# LLVM stuff
LLVM_PATH      = "/Users/matt/Documents/programming/build/Release+Asserts"
LLVM_CONFIG    = "#{LLVM_PATH}/bin/llvm-config"

output         = `#{LLVM_CONFIG} --cppflags --ldflags --libs core`.split("\n")
LLVM_CXX_FLAGS = output[0]
LLVM_LD_FLAGS  = output[1]
LLVM_LIBRARIES = output[2]

# Base stuff
CXX_FLAGS      = "-Wall -I/opt/local/include"
LINKER_FLAGS   = "-L/opt/local/lib"

# core compiler/linker stuff
CXX            = "#{LLVM_PATH}/bin/clang++ #{CXX_FLAGS} #{LLVM_CXX_FLAGS}"
LINKER         = "#{LLVM_PATH}/bin/clang++ #{LINKER_FLAGS} #{LLVM_LD_FLAGS} #{LLVM_LIBRARIES}"