CFLAGS = -Wall -Wextra -pedantic -std=c++11
CFLAGS += -g
.PHONY: all clean
.SILENT: count


IPC_WITH_HEADER = SemaphoreArray.o Semaphore.o Socket.o
IPC_WITHOUT_HEADER = Queue.o SharedMemory.o
IPC = $(IPC_WITHOUT_HEADER) $(IPC_WITH_HEADER)
HELPER = Config.o Helper.o
INTERFACES = iRobot1aParteArmado.o  iRobot1aParteSacado.o  iRobot2aParte.o
COMPONENTES = ExclusionRobots Plataforma
MODELO = dispositivo robot1aParteArmado robot1aParteSacado robot2aParte
UTILITARIOS = iniciadorCliente iniciadorIdManager iniciadorBroker terminador
WRAPPERS = net-wrapper-activado net-wrapper-armado net-wrapper-dispositivo net-wrapper-salida net-wrapper-broker net-wrapper-syncMessage net-wrapper-idm
NETWORK = net-receiver net-sender $(WRAPPERS) net-unwrapper broker-sender broker-receiver
RING = ArmarAnillo.o
IDM = net-idManager 
BROKER = broker
programs = $(COMPONENTES) $(MODELO) $(UTILITARIOS) $(NETWORK) $(IDM) $(BROKER)
objects = $(HELPER) $(IPC) $(INTERFACES) $(RING)

all: $(objects) $(programs)

count:
	echo -n "Objects: "
	echo $(objects) | wc -w
	echo -n "Programs: "
	echo $(programs) | wc -w

clean:
	$(RM) $(objects) $(programs) ids.dat broker.dat

$(RING): %.o : %.cpp %.h Socket.h includes.h
	g++ -c -o $@ $< $(CFLAGS)

$(BROKER): % : %.cpp $(IPC) $(HELPER) $(RING) includes.h net-idManagerProtocol.h
	g++ -o $@ $< $(HELPER) $(IPC)  $(RING) $(CFLAGS)

$(NETWORK): % : %.cpp Socket.o Queue.o $(HELPER) 
	g++ -o $@ $< $(HELPER) Socket.o Queue.o $(CFLAGS)

$(IDM): % : %.cpp includes.h net-idManagerProtocol.h $(IPC) $(HELPER)
	g++ -o $@ $< $(HELPER) $(IPC) $(CFLAGS)

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
