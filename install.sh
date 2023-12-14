#!/bin/bash

# Note: This script is run by the Makefile (make install), you don't need to run it

case $1 in
install)
	cp -v libNexus/util/memfiller/memfiller.x /usr/local/bin/memfiller
	cp -v libNexus/util/lock/lock.x /mnt/sda1
	cp -v libNexus/util/lock/lock.x /usr/local/bin/lock
	cp -v nexus/nexus.x /mnt/sda1
	cp -v nexus/nexus.x /usr/local/bin/nexus
	cp -v libNexus/libNexus.so /usr/local/lib
	cp -v libNexus/libNexus.so /mnt/sda1
	cp -v testGame/testGame.x /usr/local/bin/testGame
	[ ! -d /root/.nexus ] && mkdir -v /root/.nexus
	cp -fv nexus/nexus.cfg /root/.nexus
	touch /root/.nexus/.sensors.output
	touch /root/.nexus/.bell.output
	cp -rfv nexus/mesh /root/.nexus
	echo "rem to run ldconfig for a new installation" >/dev/null
	;;
uninstall)
	rm -v /usr/local/bin/{nexus,lock,testGame,memfiller}
	rm -v /usr/local/lib/libNexus.so
	[ -d /root/.nexus ] && rm -rv /root/.nexus
	;;
*)
	echo "This script should be run from the makefile, exiting"
	;;
esac
