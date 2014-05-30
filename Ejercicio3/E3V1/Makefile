C++ = g++
DFLAGS = -Wall -Wextra -ansi -pedantic-errors
#OFLAGS = -O3
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = 
PROGRAMAS = productor encargadoDiscos encargadoProcesadores encargadoMotherboards
LANZADORES = iniciador terminador lanzador
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES)

IPC_CON_HEADER =
IPC_SIN_HEADER = Queue.o Socket.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)

INTERFACES = iProductor.o iEncargado.o
MODELO = 
ACCESORIOS = Helper.o Config.o
OBJS = $(IPC) $(INTERFACES) $(MODELO) $(ACCESORIOS)

all: rpc $(OBJS) $(PROGS) 

rpc: RPC/idManager.x RPC/idManager_server.c
	make -C RPC --no-print-directory

# Ejecutables

$(PROGRAMAS): %: %.cpp $(ACCESORIOS) Queue.o includes.h Config.o Helper.o
	$(C++) -o $@ $< $(CFLAGS) $(INTERFACES) Queue.o Config.o Helper.o

$(COMPONENTES): %: %.cpp $(IPC) Helper.o includes.h Config.o
	$(C++) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o Config.o

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o

# Objetos

$(MODELO): %.o: %.cpp %.h includes.h Helper.h Config.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h includes.h Helper.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPC_CON_HEADER) $(ACCESORIOS): %.o: %.cpp %.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPC_SIN_HEADER): %.o: %.cpp
	$(C++) -c -o $@ $< $(CFLAGS)

# Dependencias especificas

productor: iProductor.o
encargadoDiscos: iEncargado.o
encargadoProcesadores: iEncargado.o
encargadoMotherboards: iEncargado.o

# Limpieza

clean:
	rm -f *.o $(PROGS)
	make -C RPC --no-print-directory clean
