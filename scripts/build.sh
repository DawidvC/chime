#!/usr/bin/env bash

set -e

# emit IR
/tmp/chime/chime -e -o /tmp/chime/target.ll $1

# compile to object code
llc -filetype=obj -o /tmp/chime/target.o /tmp/chime/target.ll

# link
clang -o /tmp/chime/target -L/tmp/chime -lchimeruntime -lchime /tmp/chime/target.o

# execute
/tmp/chime/target
