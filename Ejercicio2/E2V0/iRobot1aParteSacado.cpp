/*
 * File:   iRobot1aParteSacado.cpp
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:50 PM
 */

#include "iRobot1aParteSacado.h"
#include "Helper.h"

iRobot1aParteSacado::iRobot1aParteSacado(unsigned number) {
	this->id = number * 2 + 2;
	this->number = number;
	colaActivado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO, "iRobot1aParteSacado");
	colaActivado->get();
	colaSalida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA, "iRobot1aParteSacado");
	colaSalida->get();
	toPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_INTERFACE, "iRobot1aParteSacado");
	toPlataforma->get();
	fromPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_INTERFACE, "iRobot1aParteSacado");
	fromPlataforma->get();
	toExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::EXCLUSION_FROM_INTERFACE, "iRobot1aParteSacado");
	toExclusion->get();
	fromExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::EXCLUSION_TO_INTERFACE, "iRobot1aParteSacado");
	fromExclusion->get();
}

struct dispositivo iRobot1aParteSacado::esperarDispositivo() {
	ColaPlataforma::message msg;
	msg.mtype = id;
	msg.operation = ColaPlataforma::ESPERAR_DISPOSITIVO;
	toPlataforma->send(msg);
	msg = fromPlataforma->receive(id);
	return msg.dispositivo;
}

void iRobot1aParteSacado::esperarSiArmando() {
	ColaExclusion::message msg;
	msg.mtype = id;
	msg.operation = ColaExclusion::ESPERAR_SI_ARMANDO;
	msg.number = number;

	toExclusion->send(msg);
	msg = fromExclusion->receive(id);
	if (msg.operation != ColaExclusion::ESPERAR_SI_ARMANDO) {
		Helper::output(stderr, "iRobot1aParteSacado esperaba ESPERAR_SI_ARMANDO\n", Helper::Colours::RED);
	}
}

struct dispositivo iRobot1aParteSacado::tomarDispositivo(struct dispositivo d) {
	ColaPlataforma::message msg;
	msg.mtype = id;
	msg.operation = ColaPlataforma::TOMAR_DISPOSITIVO;
	msg.dispositivo = d;
	toPlataforma->send(msg);
	msg = fromPlataforma->receive(id);
	if (msg.operation != ColaPlataforma::TOMAR_DISPOSITIVO) {
		Helper::output(stderr, "iRobot1aParteSacado esperaba TOMAR_DISPOSITIVO\n", Helper::Colours::RED);
	}
	if (msg.dispositivo.id != d.id) {
		Helper::output(stderr, "iRobot1aParteSacado llego un dispositivo distinto del solicitado\n", Helper::Colours::RED);
	}
	return msg.dispositivo;
}

void iRobot1aParteSacado::colocarDispositivo(struct dispositivo d) {
	ColaSalida::message msg;
	msg.mtype = d.tipo;
	msg.dispositivo = d;
	colaSalida->send(msg);
}

void iRobot1aParteSacado::avisarSiEsperandoParaArmar() {
	ColaExclusion::message msg;
	msg.mtype = id;
	msg.operation = ColaExclusion::AVISAR_SI_ESPERANDO_PARA_ARMAR;
	msg.number = number;

	toExclusion->send(msg);
}
