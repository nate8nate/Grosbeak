#!/bin/sh

cc src/*.cpp -Wall -Wextra -O0 -g -std=c++11 -F/Library/Frameworks/ -framework SDL2 -framework OpenGL -o build/G
