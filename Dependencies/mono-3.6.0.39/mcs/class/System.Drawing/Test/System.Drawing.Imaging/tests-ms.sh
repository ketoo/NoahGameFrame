#!/bin/sh

if [ $# -eq 0 ]; then
	echo "You should give a list of test names such as: "
	echo "$0 System.Drawing.TestStringFormat"
	echo "or"
	echo "$0 all"	
	exit 1
fi

export MSNet=Yes
cp ../../System.Drawing_test.dll .
topdir=../../../..
NUNITCONSOLE=$topdir/class/lib/nunit-console.exe
MONO_PATH=$topdir/nunit20:$topdir/class/lib:.

for i in $@; do
	if [ "$i" = "all" ]; then
		fixture=""
	else
		fixture="/fixture:MonoTests.${i}"
	fi
	MONO_PATH=$MONO_PATH \
		${NUNITCONSOLE} System.Drawing_test.dll $fixture
done



