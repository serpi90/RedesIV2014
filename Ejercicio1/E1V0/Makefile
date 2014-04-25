C++ = g++
DFLAGS = -Wall -g
#OFLAGS = -O3
#LFLAGS = -lpthread
CFLAGS = $(DFLAGS) $(OFLAGS) $(LFLAGS)

PROGS = plataforma terminator RobotA RobotB shmstat
IPC = Queue.o SemaphoreArray.o Semaphore.o SharedMemory.o
OBJS = $(IPC) iRobotA.o iRobotB.o Plataforma.o

all: $(PROGS)

shmstat: shmstat.cpp includes.h SharedMemory.o Semaphore.o SemaphoreArray.o
	$(C++) -o $@ $< SharedMemory.o Semaphore.o SemaphoreArray.o $(CFLAGS)

RobotA: RobotA.cpp iRobotA.o includes.h $(IPC)
	$(C++) -o $@ $< iRobotA.o $(IPC) $(CFLAGS)

RobotB: RobotB.cpp iRobotB.o includes.h $(IPC)
	$(C++) -o $@ $< iRobotB.o $(IPC) $(CFLAGS)

plataforma: plataforma.cpp Plataforma.o $(IPC) includes.h
	$(C++) -o $@ $< Plataforma.o $(IPC) $(CFLAGS)
	
terminator: terminator.cpp $(IPC) includes.h
	$(C++) -o $@ $< $(IPC) $(CFLAGS)

$(OBJS): %.o: %.cpp includes.h
	$(C++) -o $@ $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(PROGS)

