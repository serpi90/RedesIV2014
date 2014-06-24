CFLAGS = -Wall -Wextra -pedantic -std=gnu++11
CFLAGS += -g
.PHONY: all clean

IPC_WITH_HEADER = SemaphoreArray.o Semaphore.o
IPC_WITHOUT_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_WITHOUT_HEADER) $(IPC_WITH_HEADER)
HELPER = Config.o Helper.o
INTERFACES = iRobot1aParteArmado.o  iRobot1aParteSacado.o  iRobot2aParte.o
COMPONENTES = ExclusionRobots Plataforma
MODELO = dispositivo robot1aParteArmado robot1aParteSacado robot2aParte
UTILITARIOS = iniciador lanzador terminador
programs = $(COMPONENTES) $(MODELO) $(UTILITARIOS)
objects = $(HELPER) $(IPC) $(INTERFACES)

all: $(objects) $(programs)

clean:
	$(RM) $(objects) $(programs)

$(COMPONENTES) $(UTILITARIOS): % : %.cpp includes.h $(HELPER) $(IPC)
	g++ -o $@ $< $(HELPER) $(IPC) $(CFLAGS)

dispositivo: dispositivo.cpp includes.h $(HELPER) Queue.o 
	g++ -o $@ $< $(HELPER)  Queue.o $(CFLAGS)

robot1aParteArmado: robot1aParteArmado.cpp includes.h $(HELPER) Queue.o iRobot1aParteArmado.o
	g++ -o $@ $< $(HELPER) Queue.o iRobot1aParteArmado.o $(CFLAGS)

robot1aParteSacado: robot1aParteSacado.cpp includes.h $(HELPER) Queue.o iRobot1aParteSacado.o
	g++ -o $@ $< $(HELPER) Queue.o iRobot1aParteSacado.o $(CFLAGS)

robot2aParte: robot2aParte.cpp includes.h $(HELPER) Queue.o iRobot2aParte.o
	g++ -o $@ $< $(HELPER) Queue.o iRobot2aParte.o $(CFLAGS)

$(INTERFACES): %.o: %.cpp %.h Queue.cpp includes.h
	g++ -c -o $@ $< $(CFLAGS)

$(HELPER) $(IPC_WITH_HEADER): %.o: %.cpp %.h
	g++ -c -o $@ $< $(CFLAGS)

$(IPC_WITHOUT_HEADER): %.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS)

Semaphore.o: SemaphoreArray.h
