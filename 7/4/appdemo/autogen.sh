#!/bin/sh

echo "aclocal.."
aclocal --force			|| exit 1
echo "autoheader.."
autoheader --force		|| exit 1
echo "automake.."
automake --force --add-missing	|| exit 1
echo "autoconf.."
autoconf --force		|| exit 1


