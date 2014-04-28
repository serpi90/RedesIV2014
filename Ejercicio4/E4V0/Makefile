C++ = g++
DFLAGS = -Wall -Wextra -ansi -pedantic-errors -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

CPROGS = comp-sala comp-cablecarril
IPROGS = sala cablecarril persona
LPROGS = lanzador terminador test
PROGS = $(CPROGS) $(IPROGS) $(LPROGS)
IPCH = Semaphore.o SemaphoreArray.o
IPCC = Queue.o SharedMemory.o
IPC = $(IPCH) $(IPCC)
IOBJS =  iCableCarril.o iSala.o
COBJS = CableCarril.o Sala.o
HOBJS = Helper.o Config.o
OBJS = $(IOBJS) $(COBJS) $(HOBJS)

all: $(IPC) $(OBJS) $(PROGS)

$(IPROGS): %: %.cpp $(IOBJS) Queue.o Helper.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(IOBJS) Queue.o Helper.o

$(CPROGS): %: %.cpp $(OBJS) $(IPC) Helper.o includes.h
	$(C++) -o $@ $< $(CFLAGS) $(COBJS) $(IPC) Helper.o

$(LPROGS): %: %.cpp $(IPC) Helper.o includes.h Config.o
	$(C++) -o $@ $< $(CFLAGS) $(IPC) Config.o

$(HOBJS): %.o: %.cpp %.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IOBJS): %.o: %.cpp %.h includes.h Helper.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(COBJS): %.o: %.cpp %.h includes.h Helper.h Config.o
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPCH): %.o: %.cpp %.h
	$(C++) -c -o $@ $< $(CFLAGS)

$(IPCC): %.o: %.cpp
	$(C++) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(IPC) $(OBJS) $(PROGS) Helper.o Config.o

