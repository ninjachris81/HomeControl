#!/bin/bash

PID=$(pidof HomeControl_Core)
printf "q\r" > /proc/$PID/fd/0

sleep 2

/usr/bin/killall -9 cat
