#! /bin/bash
aclocal
gettextize -f
# The next line should be uncommented when the libude library be done
# libtoolize --force 
autoheader
automake -a
autoconf
./configure $*
