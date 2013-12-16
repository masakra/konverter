.#!/bin/sh

TARGET="konverter"
MODULES="sql"
GMAKE="/usr/local/bin/gmake"
QMAKE="/usr/local/bin/qmake-qt4"
COMPILER=clang
CXX_FLAGS="-m64 -mmmx -msse -msse2 -msse3"
INCLUDEPATH="../../naragui"
LIBS="-L../../naragui -lnaragui"

${GMAKE} distclean

${QMAKE} -project

if [ -e ${TARGET}.pro ]
then
	# modules
	echo "QT += ${MODULES}" >> ${TARGET}.pro;
	echo "modules \"${MODULES}\" was added.";
	${QMAKE} -spec freebsd-${COMPILER};
	# C++ flags
	echo "QMAKE_CXXFLAGS += ${CXX_FLAGS}" >> ${TARGET}.pro;
	echo "C++ flags \"${CXX_FLAGS}\" was added.";
	# includepath
	echo "INCLUDEPATH += ${INCLUDEPATH}" >> ${TARGET}.pro;
	echo "include += ${INCLUDEPATH}";
	# defines
	#echo "DEFINES += ${DEFINES}" >> ${TARGET}.pro;
	#echo "defines += ${DEFINES}";
	# libraries
	echo "LIBS += ${LIBS}" >> ${TARGET}.pro;
	echo "libraries += ${LIBS}";
	${QMAKE} -spec freebsd-${COMPILER};
else
	echo "ERROR: file ${TARGET}.pro not found."
fi

