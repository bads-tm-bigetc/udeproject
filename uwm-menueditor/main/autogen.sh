#!/bin/sh
aclocal --force && libtoolize -fi && autoreconf -fis # && configure "$@"
