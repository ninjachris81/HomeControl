#!/bin/bash

if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

cp -r /tmp/SharedLib .
cp -r /tmp/HomeControl_Core .
