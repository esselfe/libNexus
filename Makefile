
default: libNexus nexus inventory testGame
	echo "################################" >/dev/null

libNexus:
	make -C libNexus
	echo "==================================">/dev/null
	echo "=======libNexus done==============">/dev/null

nexus: libNexus
	make -C nexus
	echo "==================================">/dev/null
	echo "========nexus done================">/dev/null

inventory: libNexus
	make -C inventory
	echo "==================================">/dev/null
	echo "========inventory done============">/dev/null

testGame: libNexus
	make -C testGame
	echo "==================================">/dev/null
	echo "=========testGame done============">/dev/null

install:
	./install.sh install

uninstall:
	./install.sh uninstall
