#!/bin/sh

OS=`uname -s`

case $OS in
Darwin)
	echo
	echo "macOS not supported!"
	echo
	exit 1
	;;
Linux)
	export CFLAGS="-I/usr/include/mariadb"
	export CPPFLAGS="-I/usr/include/mariadb"
	CURL_LIBS=$(curl-config --static-libs)
	export LIBS="-lmariadbclient -lssl -lcrypto -lpthread -ldl -lz $CURL_LIBS"
	;;
esac


make -C Release clean
make -C Release all
