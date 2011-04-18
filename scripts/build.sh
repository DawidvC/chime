# emit IR
/tmp/chime/chime -e -o /tmp/chime/target.ll $1

# compile to object code
llc -filetype=obj -o /tmp/chime/target.o /tmp/chime/target.ll

# link
clang -o /tmp/chime/target -L/tmp/chime -lchimeruntime /tmp/chime/target.o

# execute
/tmp/chime/target
