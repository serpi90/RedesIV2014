DFLAGS = -Wall -Wextra -ansi -pedantic-errors
# -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = registro
PROGRAMAS = escritor lector
LANZADORES = iniciador terminador lanzador shmstat
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES)

IPC_CON_HEADER = Semaphore.o SemaphoreArray.o
IPC_SIN_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)

INTERFACES = iLector.o iEscritor.o
MODELO = Registro.o
ACCESORIOS = Helper.o Config.o
OBJS = $(IPC) $(INTERFACES) $(MODELO) $(ACCESORIOS) Interfaz.o

all: $(OBJS) $(PROGS)

.PHONY: all clean

# Ejecutables

$(PROGRAMAS): %: %.cpp $(ACCESORIOS) Queue.o includes.h Config.o Helper.o
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(INTERFACES) Queue.o Config.o Helper.o Interfaz.o

$(COMPONENTES): %: %.cpp $(IPC) Helper.o includes.h Config.o
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o Config.o

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(IPC) Config.o Helper.o

# Objetos

$(MODELO): %.o: %.cpp %.h includes.h Helper.o Config.o
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h includes.h Helper.o Interfaz.o
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(IPC_CON_HEADER) $(ACCESORIOS): %.o: %.cpp %.h
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)
	
Interfaz.o: Interfaz.cpp Interfaz.h Queue.o Helper.o
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

$(IPC_SIN_HEADER): %.o: %.cpp
	$(LINK.cpp) -c -o $@ $< $(CFLAGS)

# Dependencias especificas

Semaphore.o: SemaphoreArray.o
lanzador: Helper.o
registro: Registro.o
lector: iLector.o
escritor: iEscritor.o

# Limpieza

clean:
	rm -f *.o $(PROGS)

