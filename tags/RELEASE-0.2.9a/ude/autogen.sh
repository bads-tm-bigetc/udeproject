#! /bin/bash

echo "============================== cleaning up possibly improper checkins"
mv Makefile.am Makefile.am.origcvs
echo "*** Makefile.am saved to Makefile.am.origcvs"
cat Makefile.am.origcvs | sed '
v
/\\$/N
/SUBDIRS[[:space:]]*=/ {
s/\([=]\|[[:space:]]\)intl\([[:space:]]\|$\)/\1/g
s/\([=]\|[[:space:]]\)m4\([[:space:]]\|$\)/\1/g
s/\([^\n \t]\)[\t ]\{2,\}/\1 /g
}
/EXTRA_DIST[[:space:]]*=/ {
s/\([=]\|[[:space:]]\)mkinstalldirs\([[:space:]]\|$\)/\1/g
s/\([=]\|[[:space:]]\)config\.rpath\([[:space:]]\|$\)/\1/g
s/\([^\n \t]\)[\t ]\{2,\}/\1 /g
}
/ACLOCAL_AMFLAGS[[:space:]]*=/ {
d
}
' > Makefile.am
mv configure.in configure.in.origcvs
echo "*** configure.in saved to configure.in.origcvs"
cat configure.in.origcvs | sed '
v
/\\$/N
/AC_OUTPUT[[:space:]]*(/ {
s/\([=]\|[[:space:]]\)intl\/Makefile\([[:space:]]\|)\)/\1/g
s/\([=]\|[[:space:]]\)po\/Makefile\.in\([[:space:]]\|)\)/\1/g
s/\([=]\|[[:space:]]\)po\/Makefile\([[:space:]]\|)\)/\1/g
s/\([=]\|[[:space:]]\)m4\/Makefile\([[:space:]]\|)\)/\1/g
s/\([^\n \t]\)[\t ]\{2,\}/\1 /g
}
' > configure.in

echo "============================== creating some symlinks"
ln -s ../README doc

echo "============================== gettextize"
gettextize -f --intl --no-changelog

echo "============================== aclocal"
aclocal -I m4
# The next two lines should be uncommented when the libude library be done
# echo "============================== libtoolize"
# libtoolize --force 

echo "============================== autoheader"
autoheader

echo "============================== automake"
automake -a

echo "============================== autoconf"
autoconf

echo "============================== configure"
./configure $*
