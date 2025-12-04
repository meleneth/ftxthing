#!/usr/bin/bash

set -euo pipefail

cmake -S . -B build-clang -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

copy build-clang/compile_commands.json .
