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
COMPILER_FLAGS     = "-Wall -Werror -Wno-unused-function #{GTEST_CXX_FLAGS} -I#{Rake.original_dir} -I#{LLVM_PATH}/include -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS"
LINKER_FLAGS       = ""

# core compiler/linker stuff
CLANG_PATH         = `which clang`.chomp
CC                 = "#{CLANG_PATH} #{COMPILER_FLAGS}"
CXX                = "#{CLANG_PATH}++ #{COMPILER_FLAGS}"
LINKER             = "#{CLANG_PATH}++ #{LINKER_FLAGS}"

# chime compiler stuff
CHIME_LIBRARIES    = "-L#{BUILD_PATH} -lchimeruntime -lchime"
CHIME_COMPILER     = "#{BUILD_PATH}/chime"

# -r : add object files to the archive
# -s : add object-file index
AR_PATH            = `which ar`.chomp
ARCHIVER           = "#{AR_PATH} rs"

