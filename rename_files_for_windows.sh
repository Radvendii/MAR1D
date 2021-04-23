#!/usr/bin/env bash
#
# safety first
set -euo pipefail

# don't interepret weird file names as globs
set -o noglob

cd ${MESON_SOURCE_ROOT}/resources/2D

# "./$f" is needed to avoid interpreting -_0.png as an option
# ${f::1} takes the first char
# printf "%d" "'X" prints the ascii value of X
# ${f:1} appends the rest of the string back on
for f in $(ls -A); do
    mv "./$f" "$(printf "%d" "'${f::1}")${f:1}"
done
