#!/bin/bash

rm -f swaybar_fetch swaybar_fetch_debug

clear

g++ \
  -I. \
  -DDEBUG \
  -include Debug.h \
  -g -O0 -fsanitize=address,undefined \
  -o swaybar_fetch_debug \
  *.cpp

echo "-- DEBUG -------------------------------------------------------------------------------------------------------------"

./swaybar_fetch_debug