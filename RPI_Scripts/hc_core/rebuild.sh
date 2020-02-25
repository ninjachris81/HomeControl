#!/bin/bash

if [[ $(id -u) -ne 0 ]] ; then echo "Please run as root" ; exit 1 ; fi

LIBFILE=libSharedLib.a
EXEFILE=HomeControl_Core
CPUCORES=1

systemctl stop hc_core

cd /opt/hc/SharedLib
rm $LIBFILE
qmake
make -j$CPUCORES

if [[ ! -f $LIBFILE ]] ; then
    echo 'File $LIBFILE could not be created, aborting.'
    exit
fi

cp $LIBFILE /usr/lib/$LIBFILE

cd /opt/hc/HomeControl_Core
rm $EXEFILE
qmake
make -j$CPUCORES

if [[ ! -f $EXEFILE ]] ; then
    echo 'File $EXEFILE is not there, aborting.'
    exit
fi

systemctl start hc_core
