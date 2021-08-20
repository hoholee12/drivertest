#!/bin/bash

make
rmmod hello 2>/dev/null
insmod hello.ko
