#!/bin/sh
rm -rf urdb2gtkd # need this since i made a mistake when checkin in the project
                 # for the first time, sorry...
aclocal
automake -a
autoconf
