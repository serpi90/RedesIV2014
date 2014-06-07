SERVER = idManager_server
SOURCES.x = idManager.x
TARGETS = idManager.h idManager_xdr.c idManager_clnt.c idManager_svc.c idManager_server.c

CFLAGS = -Wno-unused-variable
LDFLAGS = -lnsl

all : idManager_xdr.o idManager_svc.o idManager_clnt.o idManager_server.o idManager.h idManager_server
	touch ids.dat

.PHONY: all clean

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(SOURCES.x)

idManager_xdr.o idManager_svc.o idManager_clnt.o: %.o : %.c idManager.h

idManager_server.o: idManager_server.c idManager.h
	$(LINK.c) -c -o $@ $< -Wall -Wextra -ansi -Wno-unused-parameter

idManager_server : idManager_server.o idManager_xdr.o idManager_svc.o idManager.h
	$(LINK.c) -o idManager_server idManager_server.o idManager_xdr.o idManager_svc.o $(LDLIBS)

 clean:
	 $(RM) *.o idManager_server ids.dat

