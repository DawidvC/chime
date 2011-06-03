#!/usr/bin/env bash

set -e

# compile to object code
/tmp/chime/chime -o /tmp/chime/target.o $1

# link
#clang -o /tmp/chime/target -L/tmp/chime -lchimeruntime -lchime /tmp/chime/target.o
LD_FLAGS="-dynamic -arch x86_64 -macosx_version_min 10.6.0 -lcrt1.10.6.o -lSystem"
ld $LD_FLAGS -L/tmp/chime -lchimeruntime -lchime -o /tmp/chime/target /tmp/chime/target.o

# execute
/tmp/chime/target
