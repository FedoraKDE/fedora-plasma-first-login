#!/usr/bin/env bash

$EXTRACTRC `find . -name '*.ui' -o -name '*.rc'` >> rc.cpp
$XGETTEXT rc.cpp `find . -name '*.cpp'` -o $podir/fedora-plasma-first-login.pot
$XGETTEXT `find . -name '*.qml'` -j -L Java -o $podir/fedora-plasma-first-login.pot
rm -f rc.cpp
