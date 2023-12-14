DST= libNexus.so

INCLUDEDIR= ./include
CFLAGS= -D_GNU_SOURCE -Wall -Werror -std=c99 -pedantic
LDFLAGS= -lc -lm -lglut -lsensors -lX11

OBJCMD= gcc -c

default:
	make -C config
	echo "*******************" >/dev/null
	make -C hud
	echo "*******************" >/dev/null
	make -C Ninit
	echo "*******************" >/dev/null
	make -C modes
	echo "*******************" >/dev/null
	make -C objects
	echo "*******************" >/dev/null
	make -C scene
	echo "*******************" >/dev/null
	make -C state
	echo "*******************" >/dev/null
	make -C util
	echo "*******************" >/dev/null
	make -C X11
	echo "*******************" >/dev/null
	$(OBJCMD)  events.c $(CFLAGS) -I$(INCLUDEDIR) -o events.o
	$(OBJCMD)  getargs.c $(CFLAGS) -I$(INCLUDEDIR) -o getargs.o
	$(OBJCMD)  libNexus.c $(CFLAGS) -I$(INCLUDEDIR) -o libNexus.o
	$(OBJCMD)  lexic/lexic.c $(CFLAGS) -I$(INCLUDEDIR) -o lexic/lexic.o
	$(OBJCMD)  mesh.c $(CFLAGS) -I$(INCLUDEDIR) -o mesh.o
	$(OBJCMD)  meshEditing.c $(CFLAGS) -I$(INCLUDEDIR) -o meshEditing.o
	$(OBJCMD)  Nsignals.c $(CFLAGS) -I$(INCLUDEDIR) -o Nsignals.o
	$(OBJCMD)  Ntype.c $(CFLAGS) -I$(INCLUDEDIR) -o Ntype.o
	$(OBJCMD)  selectID.c $(CFLAGS) -I$(INCLUDEDIR) -o selectID.o
	$(OBJCMD)  threading.c $(CFLAGS) -I$(INCLUDEDIR) -o threading.o
	echo "*******************" >/dev/null
	gcc -shared -I$(INCLUDEDIR) $(CFLAGS) \
		$(LDFLAGS) \
	config/config.o	\
	events.o getargs.o \
	hud/{commands,hud,menu,messages}.o \
	lexic/lexic.o libNexus.o Ninit/Ninit.o \
	mesh.o meshEditing.o \
	modes/{browser,buffer,editor,memoryViewer,race}.o \
	Nsignals.o Ntype.o objects/objects.o selectID.o \
	scene/{camera,map,osdata,displayList,drawing}.o \
	state/{state,restore}.o \
	threading.o util/{tester/tester,util}.o \
	X11/NX.o \
	-o$(DST)
	cp libNexus.so /usr/local/lib
	rm *.o {config,hud,Ninit,modes,objects,scene,state,util{,/tester,/lock}}/*.o
