#!/bin/bash

make
rmmod hello 2>/dev/null
insmod hello.ko paramTest=2 paramArray=1,2,3
