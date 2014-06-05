DFLAGS = -Wall -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

PROGS = plataforma terminator RobotA RobotB shmstat
IPC = Queue.o SemaphoreArray.o Semaphore.o SharedMemory.o
OBJS = $(IPC) iRobotA.o iRobotB.o Plataforma.o

all: $(OBJS) $(PROGS)

.PHONY: all clean

$(PROGS): %: %.cpp $(IPC) includes.h
	$(LINK.cpp) -o $@ $< $(OBJS) $(CFLAGS)

$(OBJS): %.o: %.cpp includes.h
	$(LINK.cpp) -o $@ $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(PROGS)

RobotA: iRobotA.o
RobotB: iRobotB.o
plataforma: Plataforma.o
