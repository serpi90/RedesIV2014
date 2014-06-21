CFLAGS = -Wall -Wextra -pedantic -std=gnu++11
.PHONY: all clean

IPC_WITH_HEADER = SemaphoreArray.o Semaphore.o
IPC_WITHOUT_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_WITHOUT_HEADER) $(IPC_WITH_HEADER)
HELPER = Config.o Helper.o
INTERFACES = iRobot1aParteArmado.o  iRobot1aParteSacado.o  iRobot2aParte.o
COMPONENTES = ExclusionRobots Plataforma 
programs = $(COMPONENTES)
objects = $(HELPER) $(IPC) $(INTERFACES)

all: $(objects) $(programs)

clean:
	$(RM) $(objects) $(programs)

$(COMPONENTES): % : %.cpp includes.h $(HELPER) $(IPC)
	g++ -o $@ $< $(HELPER) $(IPC) $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h Queue.cpp includes.h
	g++ -c -o $@ $< $(CFLAGS)

$(HELPER) $(IPC_WITH_HEADER): %.o: %.cpp %.h
	g++ -c -o $@ $< $(CFLAGS)

$(IPC_WITHOUT_HEADER): %.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

Semaphore.o: SemaphoreArray.h
