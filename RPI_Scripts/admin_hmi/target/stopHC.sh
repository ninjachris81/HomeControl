#!/bin/bash

PID=$(pidof HomeControl_Admin_RPI)
printf "q\r" > /proc/$PID/fd/0

sleep 2

kill $PID

