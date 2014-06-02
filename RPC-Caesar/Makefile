# Parameters

TARGETS_SVC.c = caesar_svc.c caesar_xdr.c 
TARGETS_CLNT.c = caesar_clnt.c caesar_xdr.c 
TARGETS = caesar.h caesar_xdr.c caesar_clnt.c caesar_svc.c

OBJECTS_CLNT = caesar_clnt.o caesar_xdr.o
OBJECTS_SVC = caesar_svc.o caesar_xdr.o

# Compiler flags 

LDLIBS += -lnsl

# Targets 

all : client_c client_cpp server

$(TARGETS) : caesar.x 
	rpcgen caesar.x

$(OBJECTS_CLNT) : $(TARGETS_CLNT.c)

$(OBJECTS_SVC) : $(TARGETS_SVC.c)

client_c : client_c.c $(OBJECTS_CLNT) 
	gcc -c -o client_c.o client_c.c
	gcc -o client_c client_c.o $(OBJECTS_CLNT) $(LDLIBS)

client_cpp : client_cpp.cpp $(OBJECTS_CLNT)
	g++ -c -o client_cpp.o client_cpp.cpp
	g++ -o client_cpp client_cpp.o $(OBJECTS_CLNT) $(LDLIBS) 

server : server.c $(OBJECTS_SVC)
	gcc -c -o server.o server.c
	gcc -o server server.o $(OBJECTS_SVC) $(LDLIBS)

clean:
	 $(RM) core *.o client_c client_cpp server

