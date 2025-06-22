#!/usr/bin/env bash
set -e

g++ -std=c++20 -Iinclude examples/llama_example.cpp \
    src/tensor_ops.cpp src/llama_api.cpp -o llama_example
./llama_example > llama_output.txt

echo "Llama example output:" 
cat llama_output.txt
