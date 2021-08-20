#!/bin/bash

./build.sh
rmmod hello 2>/dev/null
insmod hello.ko
