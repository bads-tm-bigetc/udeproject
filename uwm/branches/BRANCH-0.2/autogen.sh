#!/bin/sh

echo "============================== creating some symlinks"
ln -s ../README doc

echo "============================== autoreconf"
autoreconf -fis || exit 1

echo "============================== configure"
./configure "$@"
