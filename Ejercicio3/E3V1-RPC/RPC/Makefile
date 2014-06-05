SERVER = idManager_server

SOURCES.x = idManager.x

TARGETS_SVC.c = idManager_svc.c idManager_xdr.c 
TARGETS_CLNT.c = idManager_clnt.c idManager_xdr.c 
TARGETS = idManager.h idManager_xdr.c idManager_clnt.c idManager_svc.c idManager_server.c

OBJECTS_CLNT = $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC =  $(TARGETS_SVC.c:%.c=%.o)
# Compiler flags 

CFLAGS = -Wno-unused-variable
LDFLAGS = -lnsl
RPCGENFLAGS = 

# Targets 

all : $(OBJECTS_CLNT) $(SERVER)
	touch ids.dat

.PHONY: all clean

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

idManager_xdr.o idManager_svc.o idManager_clnt.o: %.o : %.c idManager.h
	$(LINK.c) -c -o $@ $<

idManager_server.o: idManager_server.c idManager.h
	$(LINK.c) -c -o $@ $< -Wall -Wextra -ansi -Wno-unused-parameter

$(SERVER) : $(OBJECTS_SVC) idManager_server.o
	$(LINK.c) -o $(SERVER) idManager_server.o $(OBJECTS_SVC) $(LDLIBS)

 clean:
	 $(RM) $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER) idManager_server.o ids.dat

