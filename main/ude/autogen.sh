#! /bin/bash
ln -s ../README doc
echo "============================== gettextize"
gettextize -f --intl --no-changelog
echo "============================== aclocal"
aclocal -I m4
# The next line should be uncommented when the libude library be done
# libtoolize --force 
echo "============================== autoheader"
autoheader
echo "============================== automake"
automake -a
echo "============================== autoconf"
autoconf
echo "============================== configure"
./configure $*
