#!/bin/sh
mkdir  m4
echo "aclocal.."
aclocal --force			|| exit 1
echo "autoheader.."
autoheader --force		|| exit 1
echo "automake.."
automake --force --add-missing	|| exit 1
echo "autoconf.."
autoconf --force		|| exit 1


