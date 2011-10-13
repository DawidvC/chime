# Rakefile: configuration

# LLVM stuff
LLVM_CONFIG        = "#{LLVM_PATH}/bin/llvm-config"

# gtest stuff
GTEST_CXX_FLAGS    = "-I#{GTEST_PATH} -I#{GTEST_PATH}/include"

# core x86
output             = `#{LLVM_CONFIG} --ldflags --libs nativecodegen`.split("\n")
LLVM_LD_FLAGS      = output[0]
LLVM_LIBRARIES     = output[1]

# Base stuff
BIN_UTILS_PATH     = "/usr/bin"
COMPILER_FLAGS     = "-Wall -Werror -Wno-unused-function #{GTEST_CXX_FLAGS} -I#{Rake.original_dir} -I#{LLVM_PATH}/include -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS"
LINKER_FLAGS       = ""

# core compiler/linker stuff
CC                 = "#{BIN_UTILS_PATH}/clang #{COMPILER_FLAGS}"
CXX                = "#{BIN_UTILS_PATH}/clang++ #{COMPILER_FLAGS}"
LINKER             = "#{BIN_UTILS_PATH}/clang++ #{LINKER_FLAGS}"

# chime compiler stuff
CHIME_LIBRARIES    = "-L#{BUILD_PATH} -lchimeruntime -lchime"
CHIME_COMPILER     = "#{BUILD_PATH}/chime"

# -r : add object files to the archive
# -s : add object-file index
ARCHIVER           = "#{BIN_UTILS_PATH}/ar rs"
