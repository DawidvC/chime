# emit IR
clang -I. -S -emit-llvm -o /tmp/chime/llvm_cpp.ll $1

# decompile to cpp
llc -march=cpp /tmp/chime/llvm_cpp.ll
