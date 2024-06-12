#!/bin/bash

gcc -o c-mediaplayer main.c  -Wall -std=c99 -pedantic -I. -I./include -L./lib  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./c-mediaplayer
