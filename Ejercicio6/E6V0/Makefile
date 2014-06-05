DFLAGS = -Wall -Wextra -ansi -pedantic-errors
# -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

COMPONENTES = comp-sampler comp-analyzer shmstat
PROGRAMAS = sampler analyzer
LANZADORES = iniciador terminador lanzador
PROGS = $(COMPONENTES) $(PROGRAMAS) $(LANZADORES)

IPC_CON_HEADER = Semaphore.o SemaphoreArray.o
IPC_SIN_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_CON_HEADER) $(IPC_SIN_HEADER)

INTERFACES = iSampler.o iAnalyzer.o
MODELO = Sampler.o Analyzer.o
ACCESORIOS = Helper.o Config.o
OBJS = $(IPC) $(INTERFACES) $(MODELO) $(ACCESORIOS)

all: $(OBJS) $(PROGS)

.PHONY: all clean

# Ejecutables

$(PROGRAMAS): %: %.cpp $(ACCESORIOS) Queue.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(INTERFACES) Queue.o Helper.o Config.o

$(COMPONENTES): %: %.cpp $(IPC) Helper.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(MODELO) $(IPC) Helper.o 

$(LANZADORES): %: %.cpp $(IPC) Config.o includes.h
	$(LINK.cpp) -o $@ $< $(CFLAGS) $(IPC) Config.o

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

Semaphore.o: SemaphoreArray.h
comp-sampler: Sampler.o
comp-analyzer: Analyzer.o
sampler: iSampler.o
analyzer: iAnalyzer.o

# Limpieza

clean:
	rm -f *.o $(PROGS)

