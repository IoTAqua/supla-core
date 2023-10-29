#!/bin/sh

OS=`uname -s`

case $OS in
Darwin)
	export CFLAGS="-I/opt/local/include/openssl -I/opt/local/include"
	export CPPFLAGS="-I/opt/local/include/openssl -I/opt/local/include"
	export LDFLAGS="-L/opt/local/lib"
	export SSLDIR=/opt/local
	;;
Linux)
	export CFLAGS=
	export CPPFLAGS=
	export LDFLAGS=-static
	export SSLDIR=/usr
	;;
esac


make -C Release clean
make -C Release all
