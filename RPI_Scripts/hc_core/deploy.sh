#!/bin/bash

if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

cd /opt/hc

./copyFromTmp.sh
./rebuild.sh
