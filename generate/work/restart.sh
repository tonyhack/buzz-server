#!/bin/sh

./buzz.sh stop
rm -f *.log *.pid
./buzz.sh start
