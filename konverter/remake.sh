#!/bin/sh

TARGET="konverter"
MODULES="sql"
GMAKE="/usr/local/bin/gmake"
QMAKE="/usr/local/bin/qmake-qt4"
COMPILER=clang

${GMAKE} distclean

${QMAKE} -project

if [ -e ${TARGET}.pro ]
then
	# modules
	echo "QT += ${MODULES}" >> ${TARGET}.pro;
	echo "modules \"${MODULES}\" was added.";
	${QMAKE} -spec freebsd-${COMPILER};
else
	echo "ERROR: file ${TARGET}.pro not found."
fi

