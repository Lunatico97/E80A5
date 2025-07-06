#!/bin/bash

CODE="e8OA5"
rm "bin/$CODE"
g++ src/*.cpp "-Iinclude/" -o "bin/$CODE" -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
echo $CODE
./bin/$CODE
