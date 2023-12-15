#!/bin/bash

# Note: This script is run by the Makefile (make install), you don't need to run it

[ -d "$HOME" ] || { echo "Nexus install error: \$HOME is not set!"; exit 1; }

case $1 in
install)
	cp -v libNexus/util/memfiller/memfiller.x /usr/local/bin/memfiller
	cp -v libNexus/util/lock/lock.x /usr/local/bin/lock
	cp -v nexus/nexus.x /usr/local/bin/nexus
	cp -v libNexus/libNexus.so /usr/local/lib
	cp -v testGame/testGame.x /usr/local/bin/testGame
	[ ! -d $HOME/.nexus ] && mkdir -v $HOME/.nexus
	cp -fv nexus/nexus.cfg $HOME/.nexus
	touch $HOME/.nexus/.sensors.output
	touch $HOME/.nexus/.bell.output
	cp -rfv nexus/mesh $HOME/.nexus
	echo "rem to run ldconfig as the root user for a new installation" >/dev/null
	;;
uninstall)
	rm -v /usr/local/bin/{nexus,lock,testGame,memfiller}
	rm -v /usr/local/lib/libNexus.so
	[ -d $HOME/.nexus ] && rm -rv $HOME/.nexus
	;;
*)
	echo "This script should be run from the makefile, exiting"
	;;
esac
