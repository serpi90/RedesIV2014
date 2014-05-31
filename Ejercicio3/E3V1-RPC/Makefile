C++ = g++
DFLAGS = -Wall -Wextra -ansi -g
#OFLAGS = -O3
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = 
PROGRAMAS = productor encargadoDiscos encargadoProcesadores encargadoMotherboards
LANZADORES = iniciador terminador lanzador
CONECTIVIDAD = net-sender net-receiver net-controller
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES) $(CONECTIVIDAD)

IPC_CON_HEADER = Socket.o SemaphoreArray.o Semaphore.o
IPC_SIN_HEADER = Queue.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)

INTERFACES = iProductor.o iEncargado.o
MODELO = 
ACCESORIOS = Helper.o Config.o
OBJS = $(IPC) $(INTERFACES) $(MODELO) $(ACCESORIOS)
RPC_CLIENT = RPC/idManager_clnt.o RPC/idManager_xdr.o
all: rpc $(OBJS) $(PROGS)

# Ejecutables

rpc:
	make -C RPC --no-print-directory

$(PROGRAMAS): %: %.cpp $(ACCESORIOS) Socket.o Queue.o includes.h Config.o Helper.o $(RPC_CLIENT)
	$(C++) -o $@ $< $(CFLAGS) $(INTERFACES) Socket.o Queue.o Config.o Helper.o $(RPC_CLIENT)

$(COMPONENTES): %: %.cpp $(IPC) Helper.o includes.h Config.o
	$(C++) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o Config.o

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o

$(CONECTIVIDAD): %: %.cpp $(IPC) Config.o Helper.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o $(RPC_CLIENT)

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

net-controller: $(RPC_CLIENT)
productor: iProductor.o
encargadoDiscos: iEncargado.o
encargadoProcesadores: iEncargado.o
encargadoMotherboards: iEncargado.o

# Limpieza

clean:
	rm -f *.o $(PROGS)
	make -C RPC clean --no-print-directory
