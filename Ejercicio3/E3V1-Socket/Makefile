DFLAGS = -Wall -Wextra -ansi -pedantic-errors -g
#OFLAGS = -O3
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = 
PROGRAMAS = productor encargadoDiscos encargadoProcesadores encargadoMotherboards
LANZADORES = iniciador terminador lanzador
CONECTIVIDAD = net-sender net-receiver net-controller net-idManager
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES) $(CONECTIVIDAD)

IPC_CON_HEADER = Socket.o SemaphoreArray.o Semaphore.o
IPC_SIN_HEADER = Queue.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)

INTERFACES = iProductor.o iEncargado.o
MODELO = 
ACCESORIOS = Helper.o Config.o
OBJS = $(IPC) $(INTERFACES) $(MODELO) $(ACCESORIOS)

all: $(OBJS) $(PROGS) 

.PHONY: all clean

# Ejecutables

$(PROGRAMAS): %: %.cpp $(ACCESORIOS) Socket.o Queue.o includes.h Config.o Helper.o
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(INTERFACES) Socket.o Queue.o Config.o Helper.o

$(COMPONENTES): %: %.cpp $(IPC) Helper.o includes.h Config.o
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o Config.o

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o

$(CONECTIVIDAD): %: %.cpp $(IPC) Config.o Helper.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o

# Objetos

$(MODELO): %.o: %.cpp %.h includes.h Helper.h Config.h
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h includes.h Helper.h
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(IPC_CON_HEADER) $(ACCESORIOS): %.o: %.cpp %.h
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(IPC_SIN_HEADER): %.o: %.cpp
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

# Dependencias especificas

net-controller: net-idManagerProtocol.h
net-idManager: net-idManagerProtocol.h
iEncargado.o: net-idManagerProtocol.h
productor: iProductor.o
encargadoDiscos: iEncargado.o
encargadoProcesadores: iEncargado.o
encargadoMotherboards: iEncargado.o

# Limpieza

clean:
	rm -f *.o $(PROGS) ids.dat
