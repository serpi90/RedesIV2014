C++ = g++
DFLAGS = -Wall -Wextra -ansi -pedantic-errors
# -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = comp-sala comp-cablecarril
PROGRAMAS = sala cablecarril persona
LANZADORES = iniciador lanzador terminador
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES)
IPC_CON_HEADER = Semaphore.o SemaphoreArray.o
IPC_SIN_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)
INTERFACES =  iCableCarril.o iSala.o
MODELO = CableCarril.o Sala.o
ACCESORIOS = Helper.o Config.o
OBJS = $(INTERFACES) $(MODELO) $(ACCESORIOS)

all: $(IPC) $(OBJS) $(PROGS) shmstat

# Ejecutables

shmstat: shmstat.cpp Semaphore.o SemaphoreArray.o SharedMemory.o Helper.o includes.h
	$(C++) -o $@ $< $(CFLAGS) Semaphore.o SemaphoreArray.o SharedMemory.o Helper.o Config.o

$(PROGRAMAS): %: %.cpp $(INTERFACES) Queue.o Helper.o Config.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(INTERFACES) Queue.o Helper.o Config.o

$(COMPONENTES): %: %.cpp $(MODELO) $(IPC) Helper.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o 

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(IPC) Config.o

# Objetos

$(MODELO): %.o: %.cpp %.h includes.h Helper.h Config.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h includes.h Helper.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPC_CON_HEADER) $(ACCESORIOS): %.o: %.cpp %.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPC_SIN_HEADER): %.o: %.cpp
	$(C++) -c -o $@ $< $(CFLAGS)

# Limpieza

clean:
	rm -f $(IPC) $(OBJS) $(PROGS) Helper.o Config.o

