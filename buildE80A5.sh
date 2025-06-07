#!/bin/bash

CODE="e8OA5"
rm "bin/$CODE"
g++ src/*.cpp "-Iinclude/" -o "bin/$CODE"
echo $CODE
./bin/$CODE
